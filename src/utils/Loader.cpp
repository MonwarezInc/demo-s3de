/*
Copyright (c) 2016, Payet Thibault
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Monwarez Inc nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL PAYET THIBAULT BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "Loader.h"
using std::string;
using std::fscanf;
using std::vector;
// Some Bits operation define
#define CONFIG_MASK	1		//	00000001
#define MESH_MASK 	2		//	00000010
#define LIGHT_MASK 	4		//	00000100
#define DYN_MASK	8		//	00001000
Loader::Loader()
{
	m_state	=	0;
}
void Loader::Load(string const &filename, LoaderType type)
{
	FileManager	file (filename, "r");
	m_lastfilename =	filename;
	switch (type)
	{
		case LoaderType::CONFIG:
			this->LoadConfig(file);
			break;
		case LoaderType::MESH:
			this->LoadMesh(file);
			break;
		case LoaderType::LIGHT:
			this->LoadLight(file);
			break;
		case LoaderType::DYNAMICS:
			this->LoadDynamics(file);
			break;
	}
}
void Loader::LoadConfig(FileManager & file)
{
	this->ClearState(CONFIG_MASK);
	float	x,y,z,u,v,w,a,b,c;
	auto	fileptr	=	file.GetFilePtr();
	auto ret	=	fscanf(fileptr, "camera position(%f,%f,%f) target(%f,%f,%f) up(%f,%f,%f)\n",
												&x,&y,&z,&u,&v,&w,&a,&b,&c);
	if (ret != 9)
		throw string ("error during loading config file at: ") + m_lastfilename;
	unsigned int	width,height,fullscreen;
	ret			=	fscanf(fileptr,"resolution %ux%u\nfullscreen %u",&width,&height,&fullscreen);
	if	(ret != 3)
		throw string ("error during loading config file at: ") + m_lastfilename;
	// Now we can store the config
	m_config.position	=	glm::vec3(x,y,z);
	m_config.target		=	glm::vec3(u,v,w);
	m_config.up			=	glm::vec3(a,b,c);
	m_config.width		=	width;
	m_config.height		=	height;
	m_config.fullscreen	=	(1==fullscreen)?true:false;	
	// Update state
	m_state				|=	CONFIG_MASK;									
}
void Loader::LoadMesh(FileManager & file)
{
	this->ClearState(MESH_MASK);
	m_pMesh.clear();

	string error	=	string("error during loading meshfile: ") + m_lastfilename;
	auto fileptr	=	file.GetFilePtr();

	unsigned int	n;
	auto ret	=	fscanf(fileptr,"models %u\n",&n);
	if (ret != 1)
		throw error;
	m_pMesh.resize(n);
	for (unsigned int i = 0; i < n; ++i)
	{
		char	name[256];
		float	x,y,z,u,v,w,f;
		ret	=	fscanf(fileptr,"%255s position(%f,%f,%f) rotate(%f,%f,%f) scale(%f)",name,&x,&y,&z,
						&u,&v,&w,&f);
		if (ret != 8)
			throw error;
		fscanf(fileptr,"\n");
		m_pMesh[i].filename	=	name;
		m_pMesh[i].position	=	glm::vec3(x,y,z);
		m_pMesh[i].pitch	=	glm::vec3(glm::radians(u),glm::radians(v),glm::radians(w));
		m_pMesh[i].scale	=	f;
	}
	// Everything goes well
	m_state	|= MESH_MASK;
}
void Loader::LoadLight(FileManager & file)
{
	this->ClearState(LIGHT_MASK);
	m_vLight.clear();

	string	error	=	string("error during loading lights file: ") + m_lastfilename;
	
	auto	fileptr	=	file.GetFilePtr();	

	unsigned int	n;
	auto ret	=	fscanf(fileptr, "lights %u\n",&n);
	if (ret != 1)
		throw error;
	m_vLight.resize(n);
	for (size_t i = 0; i < n ; ++i)
	{
		float x,y,z,r,g,b,a,d,c,l,e;
		unsigned int controlpoint;
		char	curvetype[256];
		ret	=	fscanf(fileptr,
						"color(%f,%f,%f) ambiant(%f) diffuse(%f) linear(%f) constant(%f) exp(%f)\n",
						&r,&g,&b,&a,&d,&l,&c,&e);
		if (ret != 8)
			throw error;
		// Set the data in m_vLight
		m_vLight[i].color	=	glm::vec3(r,g,b);
		m_vLight[i].ambient	=	a;
		m_vLight[i].diffuse	=	d;
		m_vLight[i].linear	=	l;
		m_vLight[i].constant=	c;
		m_vLight[i].exp		=	e;

		ret	=	fscanf(fileptr, "controlpoint(%u) %255s\n", &controlpoint, curvetype);
		if (ret != 2)
			throw error;
		string	scurvetype	=	curvetype;
		m_vLight[i].vControlPoint.resize(controlpoint);
		m_vLight[i].controltype	=	scurvetype;
		if (scurvetype	==	"linear")
		{
			for (size_t j = 0; j < controlpoint; ++j)
			{
				float time;
				ret	=	fscanf(fileptr,"position(%f,%f,%f) timemill(%f)",&x,&y,&z,&time);
				fscanf(fileptr,"\n"); 
				if (ret != 4)
					throw error;
				// Set the data in vControlPoint
				m_vLight[i].vControlPoint[j].position	=	glm::vec3(x,y,z);
				m_vLight[i].vControlPoint[j].time		=	time;
			}	
		}
		else	// Do nothing
		{
			
		}
	}
	// Everything goes well
	m_state |= LIGHT_MASK;
}
void Loader::LoadDynamics(FileManager & file)
{
}
ConfigData	Loader::GetConfigData()
{
	if (0 == (m_state & CONFIG_MASK))
		throw string ("error can't get data config before loading it");
	return	m_config;
}
vector<MeshData> Loader::GetMeshData()
{
	if (0 == (m_state & MESH_MASK))
		throw string ("error can't get mesh config before loading it");
	return m_pMesh;
}
vector<LightData>	Loader::GetLightData()
{
	if (0 == (m_state & LIGHT_MASK))
		throw string ("error can't get light config before loading it");
	return	m_vLight;
}
void	Loader::ClearState(unsigned char mask)
{
	if (0 != (m_state & mask))
		m_state -= mask;
}
