#pragma once

#include <d3d11.h>
#include "ShaderBase.h"
//=====================================================================//
//! Geometry�V�F�[�_����ێ�����N���X
//=====================================================================//
class GeometryShader : public ShaderBase
{
public:
	/** Constructor */
	GeometryShader() :
		m_Interface(nullptr)
	{}

	/** Destructor */
	virtual ~GeometryShader()
	{
		if (m_Interface != nullptr)
		{
			m_Interface->Release();
			m_Interface = nullptr;
		}
	}

	/**
	* @brief GeometryShader�쐬�֐�@n
	* �w�肳�ꂽ�t�@�C������GeometryShader���쐬����@n
	* @return �쐬���� ����(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name �ǂݍ��ݑΏۂ̃t�@�C����
	*/
	virtual bool Create(ID3D11Device* device, const char*file_name)
	{
		// �t�@�C���ǂݍ���
		if (Load(file_name) == false)
		{
			return false;
		}

		// GeometryShader�쐬
		if (FAILED(device->CreateGeometryShader(
			m_Data,					// �������ƂȂ�f�[�^
			m_Size,					// �f�[�^�̃T�C�Y
			/*
				ID3D11ClassLinkage�̃|�C���^��ݒ�
					ID3D11ClassLinkage�̓V�F�[�_�̎��s�֐���
					���I�ɐ؂�ւ��邱�Ƃ��ł���
					����ɂ���āA����������V�F�[�_���ɏ����Ȃ��Ă����ނ̂ŁA
					�V�F�[�_�̑��x�ቺ��}���邱�Ƃ��ł���B
					����͎g�p���Ȃ��̂�nullptr
			*/
			nullptr,
			&m_Interface)))			// �쐬���Interface�i�[��
		{
			return false;
		}

		return true;
	}

	/** ShaderInterface�̃Q�b�^�[ */
	inline ID3D11GeometryShader* GetShaderInterface()
	{
		return m_Interface;
	}
private:
	ID3D11GeometryShader *m_Interface;		//!< @brief GeometryShaderInterface
};
