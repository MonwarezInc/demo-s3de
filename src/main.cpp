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
#include <iostream>
#include <S3DE_Engine-main.h>
#include <S3DE_Mesh.h>

#include <cstdio>
#include <cmath>
#include <sstream>
#include "utils/MemoryManager.hpp"
#include <S3DE_Interpolate.hpp>
#include <S3DE_Loader.h>
#include <S3DE_Camera.h>
#include <S3DE_CEntity.h>

#define MAX_LIGHT 	6 	// define this for now
#define	MAX_SPOT	2
struct IdMesh
{
	IdMesh()
	{
		id		=	0;
		isGood	=	false;
	}
	unsigned	int	id;
	bool			isGood;
	glm::vec3	position;
	glm::vec3	pitch;
	float		scale;
};
using namespace std;
int main (int argc, char **argv)
{
	cout << "Test engine " << endl;
	
	S3DE::CEngine	engine;
	
	// Input
	S3DE::CInput	input;

	// loader system
	S3DE::Loader	loader;
	// Some struct for loader system
 	S3DE::ConfigData		config;
	vector<S3DE::MeshData>	pmeshdata;
	// Set some default
	config.position		=	glm::vec3(350,200,300);
	config.target		=	glm::vec3(2,5,0);
	config.up			=	glm::vec3(0,0,1);
	config.width		=	640;
	config.height		=	480;
	config.fullscreen	=	false;

	try
	{
		loader.Load("./data/config",S3DE::LoaderType::CONFIG);
	}
	catch (string a)
	{
		std::cerr << a << std::endl;
	}
	std::vector<IdMesh>			vIDMesh;
	try
	{
		config	=	loader.GetConfigData();

		S3DE::EngineShader	shaderFilename	=	{"./Shader/texture.vert","./Shader/texture.frag","",""};
		S3DE::EngineWindow	windowConf	=	{config.width,config.height,config.fullscreen,"Test Engine",32,2,3,2,shaderFilename};
		
		engine.CreateWindow(windowConf);
		engine.ClearColor(0.0,0.0,0.0,1.0);
		engine.SetCameraSettings(70.0, (float)config.width/(float)config.height, 0.01, 10000);
		engine.SetCameraLocation(config.position, config.target, config.up);

		loader.Load("./data/obj.dat",S3DE::LoaderType::MESH);
		pmeshdata	=	loader.GetMeshData();	
		BasicVectorManager<S3DE::Mesh>	mesh;
		auto	nbModel	=	pmeshdata.size();
		mesh.Allocate(nbModel);
		vIDMesh.resize(nbModel);
		for (size_t	i = 0; i < nbModel ; ++i)
		{
			unsigned int	id	=	0;
			auto mesh_ptr	=	mesh.GetVectPtr(i);
			try
			{
				mesh_ptr->LoadFromFile(pmeshdata[i].filename);
				engine.AddMeshNode(mesh_ptr, id);
				vIDMesh[i].id		=	id;
				vIDMesh[i].isGood	=	true;
				vIDMesh[i].position	=	pmeshdata[i].position;
				vIDMesh[i].pitch	=	pmeshdata[i].pitch;
				vIDMesh[i].scale	=	pmeshdata[i].scale;

				engine.SetNodePosRot(vIDMesh[i].id, vIDMesh[i].position, vIDMesh[i].pitch);
				engine.SetNodeScale(vIDMesh[i].id, vIDMesh[i].scale);
			}
			catch(std::string const & a)
			{
				std::stringstream out;
				out << "Exception caught when loading: " << pmeshdata[i].filename ;
				std::cerr << out.str() << std::endl << a << std::endl;
			}
			catch (...)
			{
				throw;
			}
		}
		S3DE::Camera	camera(config.position,config.target,config.up);
		// maybe we should encapsulate timer 
		unsigned int 	start		=	SDL_GetTicks();
		unsigned int 	frametime	=	16;
		unsigned int	elapsed		=	0;
		unsigned int	totalTime	=	SDL_GetTicks() - start;
		input.GrabCursor(true);
		input.ShowCursor(false);	
		
		camera.SetSpeed(0.1);
		// Adding some light
		std::vector<S3DE::LightData>				lightdata;
		std::vector<S3DE::PointLight>				pointlight;
		std::vector<S3DE::LinearInterpolate<float>>	posintlight;
		std::vector<S3DE::SpotLight>				spotlight;
		try
		{
			loader.Load("./data/light.dat", S3DE::LoaderType::LIGHT);
			lightdata	=	loader.GetLightData();
			size_t	nlights	=	lightdata.size();
			for (size_t i = 0; i < nlights && i < MAX_LIGHT; ++i)
			{
				S3DE::PointLight	pl;
				pl.Color				=	lightdata[i].color;
				pl.AmbientIntensity		=	lightdata[i].ambient;
				pl.DiffuseIntensity		=	lightdata[i].diffuse;
				pl.Attenuation.Constant	=	lightdata[i].constant;
				pl.Attenuation.Linear	=	lightdata[i].linear;
				pl.Attenuation.Exp		=	lightdata[i].exp;
				if (lightdata[i].controltype	==	"linear")
				{
					size_t controlpoint	=	lightdata[i].vControlPoint.size();
					posintlight.push_back(S3DE::LinearInterpolate<float>());
					for (size_t j = 0; j < controlpoint; ++j)
					{
						auto vec	=	lightdata[i].vControlPoint[j].position;
						glm::vec3	position(vec.x,vec.y,vec.z);
						posintlight.back().AddPoint(position, lightdata[i].vControlPoint[j].time);
					}
				}
				else	// throw except
				{
					throw string ("Error: ") + lightdata[i].controltype + string(" curve not implemented");
				}
				// Set looped for beginning
				posintlight.back().SetLooped(true);
				pointlight.push_back(pl);
			}
			// Spot Light one for now
			glm::vec3	unit		=	glm::vec3(-1,5,-2);
			S3DE::SpotLight	sl;
			sl.Color				=	glm::vec3(1.0,1.0,0.0);
			sl.AmbientIntensity		=	0.2;
			sl.DiffuseIntensity		=	0.4;
			sl.Attenuation.Constant	=	1.0;
			sl.Attenuation.Linear	=	0.001;
			sl.Attenuation.Exp		=	0.005;
			sl.Direction			=	unit;
			sl.Cutoff				=	M_PI/16.0;
			sl.Position				=	glm::vec3(-20,50,35);
			spotlight.push_back(sl);
		}
		catch(string const &err)
		{
			std::cerr << "Uncategorized error" << std::endl;
			std::cerr << err << std::endl;
		}
		engine.AttachLight(pointlight);
		engine.AttachLight(spotlight);
		// End adding some light
		float	t	=	0;
		while (!input.terminer()) 
		{
			if (vIDMesh.size() > 9)
			{
				vIDMesh[9].pitch	=	glm::vec3(t,0,0);
			}
			for (auto & meshid: vIDMesh)
			{
				engine.SetNodePosRot(meshid.id, meshid.position, meshid.pitch);
				engine.SetNodeScale(meshid.id, meshid.scale);
			}
			auto numLight	=	pointlight.size();
			for (size_t i = 0; i < numLight; ++i)
			{
				// Apply Interpolated curve position
				auto	lightpos	=	posintlight[i].GetInterpolated(totalTime);
				pointlight[i].Position	=	lightpos;	
			}
			engine.AttachLight(pointlight);

			input.UpdateEvent();
			camera.KeyBoardEvent(input);	
			camera.Move(input,elapsed);
			unsigned int begin = SDL_GetTicks();
			// do graphical stuff
			// do animation 
			engine.SetCameraLocation(camera.GetPosition(),camera.GetTarget(),config.up);
			engine.Init();
			try
			{
				engine.Draw(totalTime);
			}
			catch(S3DE::MeshException const & me)
			{
				auto	re	=	me.GetResourceExcept();
				std::cerr << "Exception caught: " << me.what() << std::endl;
				switch(re.flag)
				{
					case	S3DE::MeshExceptFlag::FATAL:
						throw me;
						break;
					case	S3DE::MeshExceptFlag::RELEASE:
						// Do somethings
						std::cerr << "Release the node mesh id:" << re.id << std::endl;
						engine.DelMeshNode(re.id);
						if (re.id < vIDMesh.size())
							vIDMesh[re.id].isGood	=	false;
						break;
					default:
						throw me;
				}
					
			}
			elapsed = SDL_GetTicks() - begin;
			if (elapsed < frametime)
				{
					SDL_Delay(frametime - elapsed);
					elapsed	=	SDL_GetTicks() - begin;
				}
			if (input.GetTouche(SDL_SCANCODE_ESCAPE))
				break;
		totalTime	=	SDL_GetTicks() - start;
		t	+=	elapsed/1800.0;
		}
	}
	catch(string const &a)
	{
		std::cerr << "Error of type string received"<<std::endl << a << std::endl;
	}
	catch(exception * e)
	{
		std::cerr << e->what()<< std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknow type of error" << std::endl;
	}
	return EXIT_SUCCESS;
}

