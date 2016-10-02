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
#pragma once
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>
#include <S3DE_Mesh.h>
namespace S3DE
{
template <class T>
class BasicVectorManager
{
	public:
		BasicVectorManager(){}
		BasicVectorManager(size_t nb)
		{
			m_pVect.resize(nb);
			for (size_t i=0; i < nb; ++i)
				m_pVect[i]	=	std::make_unique<T>();
		}
		virtual	void	Allocate(size_t nb)
		{
			this->Release();
			m_pVect.resize(nb);
			for (size_t i=0; i < nb;++i)
				m_pVect[i]	=	std::make_unique<T>();
		}
		virtual	size_t	GetSize()
		{
			return	m_pVect.size();
		}
		virtual	void	Release()
		{
			m_pVect.clear();
		}
		virtual 		~BasicVectorManager()
		{
			this->Release();
		}
		virtual	T*	GetVectPtr(size_t i)
		{
			return m_pVect[i].get();
		}
	protected:
		std::vector<std::unique_ptr<T>>	m_pVect;
};
class BasicMeshManager : public BasicVectorManager<Mesh>
{
	public:
		BasicMeshManager();
		BasicMeshManager(size_t nb);

	protected:
};

}
