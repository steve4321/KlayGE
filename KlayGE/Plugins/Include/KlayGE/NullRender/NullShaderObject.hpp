/**
 * @file NullShaderObject.hpp
 * @author Minmin Gong
 *
 * @section DESCRIPTION
 *
 * This source file is part of KlayGE
 * For the latest info, see http://www.klayge.org
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * You may alternatively use this source under the terms of
 * the KlayGE Proprietary License (KPL). You can obtained such a license
 * from http://www.klayge.org/licensing/.
 */

#ifndef KLAYGE_PLUGINS_NULL_SHADER_OBJECT_HPP
#define KLAYGE_PLUGINS_NULL_SHADER_OBJECT_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/ShaderObject.hpp>

namespace KlayGE
{
	class NullShaderObject : public ShaderObject
	{
	public:
		NullShaderObject();

		bool AttachNativeShader(ShaderType type, RenderEffect const & effect,
			std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids, std::vector<uint8_t> const & native_shader_block) override;

		bool StreamIn(ResIdentifierPtr const & res, ShaderType type, RenderEffect const & effect,
			std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids) override;
		void StreamOut(std::ostream& os, ShaderType type) override;

		void AttachShader(ShaderType type, RenderEffect const & effect,
			RenderTechnique const & tech, RenderPass const & pass,
			std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids) override;
		void AttachShader(ShaderType type, RenderEffect const & effect,
			RenderTechnique const & tech, RenderPass const & pass, ShaderObjectPtr const & shared_so) override;
		void LinkShaders(RenderEffect const & effect) override;
		ShaderObjectPtr Clone(RenderEffect const & effect) override;

		void Bind() override;
		void Unbind() override;

	private:
		struct D3D11ShaderObjectTemplate
		{
#ifdef KLAYGE_HAS_STRUCT_PACK
#pragma pack(push, 2)
#endif
			struct D3D11ShaderDesc
			{
				D3D11ShaderDesc()
					: num_samplers(0), num_srvs(0), num_uavs(0)
				{
				}

				struct ConstantBufferDesc
				{
					ConstantBufferDesc()
						: size(0)
					{
					}

					struct VariableDesc
					{
						std::string name;
						uint32_t start_offset;
						uint8_t type;
						uint8_t rows;
						uint8_t columns;
						uint16_t elements;
					};
					std::vector<VariableDesc> var_desc;

					std::string name;
					size_t name_hash;
					uint32_t size;
				};
				std::vector<ConstantBufferDesc> cb_desc;

				uint16_t num_samplers;
				uint16_t num_srvs;
				uint16_t num_uavs;

				struct BoundResourceDesc
				{
					std::string name;
					uint8_t type;
					uint8_t dimension;
					uint16_t bind_point;
				};
				std::vector<BoundResourceDesc> res_desc;
			};
#ifdef KLAYGE_HAS_STRUCT_PACK
#pragma pack(pop)
#endif

			std::array<std::pair<std::shared_ptr<std::vector<uint8_t>>, std::string>, ST_NumShaderTypes> shader_code_;
			std::array<std::shared_ptr<D3D11ShaderDesc>, ST_NumShaderTypes> shader_desc_;
			std::array<std::shared_ptr<std::vector<uint8_t>>, ST_NumShaderTypes> cbuff_indices_;

			uint32_t vs_signature_;
		};

	public:
		explicit NullShaderObject(std::shared_ptr<D3D11ShaderObjectTemplate> const & so_template);

	private:
		std::shared_ptr<std::vector<uint8_t>> CompiteToBytecode(ShaderType type, RenderEffect const & effect,
			RenderTechnique const & tech, RenderPass const & pass, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids);
		void AttachShaderBytecode(ShaderType type, RenderEffect const & effect,
			std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids, std::shared_ptr<std::vector<uint8_t>> const & code_blob);
	private:
		std::shared_ptr<D3D11ShaderObjectTemplate> so_template_;
	};
}

#endif			// KLAYGE_PLUGINS_NULL_SHADER_OBJECT_HPP