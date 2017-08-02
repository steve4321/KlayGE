/**
 * @file NullShaderObject.cpp
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

#include <KlayGE/KlayGE.hpp>

#include <KlayGE/NullRender/NullRenderEngine.hpp>
#include <KlayGE/NullRender/NullShaderObject.hpp>

namespace KlayGE
{
	NullShaderObject::NullShaderObject()
	{
		has_discard_ = true;
		has_tessellation_ = false;
		is_shader_validate_.fill(false);
	}

	bool NullShaderObject::AttachNativeShader(ShaderType type, RenderEffect const & effect,
		std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids, std::vector<uint8_t> const & native_shader_block)
	{
		KFL_UNUSED(type);
		KFL_UNUSED(effect);
		KFL_UNUSED(shader_desc_ids);
		KFL_UNUSED(native_shader_block);
		return true;
	}

	bool NullShaderObject::StreamIn(ResIdentifierPtr const & res, ShaderType type, RenderEffect const & effect,
		std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids)
	{
		KFL_UNUSED(res);
		KFL_UNUSED(type);
		KFL_UNUSED(effect);
		KFL_UNUSED(shader_desc_ids);
		return true;
	}

	void NullShaderObject::StreamOut(std::ostream& os, ShaderType type)
	{
		KFL_UNUSED(os);
		KFL_UNUSED(type);
	}

	void NullShaderObject::AttachShader(ShaderType type, RenderEffect const & effect,
			RenderTechnique const & tech, RenderPass const & pass, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids)
	{
		KFL_UNUSED(type);
		KFL_UNUSED(effect);
		KFL_UNUSED(tech);
		KFL_UNUSED(pass);
		KFL_UNUSED(shader_desc_ids);
	}

	void NullShaderObject::AttachShader(ShaderType type, RenderEffect const & effect,
			RenderTechnique const & tech, RenderPass const & pass, ShaderObjectPtr const & shared_so)
	{
		KFL_UNUSED(type);
		KFL_UNUSED(effect);
		KFL_UNUSED(tech);
		KFL_UNUSED(pass);
		KFL_UNUSED(shared_so);
	}

	void NullShaderObject::LinkShaders(RenderEffect const & effect)
	{
		KFL_UNUSED(effect);
	}

	ShaderObjectPtr NullShaderObject::Clone(RenderEffect const & effect)
	{
		KFL_UNUSED(effect);
		return MakeSharedPtr<NullShaderObject>();
	}

	void NullShaderObject::Bind()
	{
	}

	void NullShaderObject::Unbind()
	{
	}
}
