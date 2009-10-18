// Texture.hpp
// KlayGE 纹理类 头文件
// Ver 3.9.0
// 版权所有(C) 龚敏敏, 2003-2009
// Homepage: http://klayge.sourceforge.net
//
// 3.9.0
// 隐藏了TextureLoader (2009.4.9)
// 支持Texture Array的读写 (2009.10.15)
//
// 3.8.0
// 增加了access_hint (2008.9.20)
// 增加了ElementInitData (2008.10.1)
// 多线程纹理载入 (2009.1.22)
//
// 3.6.0
// 可以通过Map直接访问纹理内容 (2007.7.7)
//
// 3.3.0
// 使用ElementFormat (2006.6.8)
//
// 3.2.0
// 支持sRGB (2006.4.24)
//
// 3.0.0
// 去掉了构造函数的usage (2005.10.5)
//
// 2.7.0
// 可以获取Mipmap中每层的宽高深 (2005.6.8)
// 增加了AddressingMode, Filtering和Anisotropy (2005.6.27)
// 增加了MaxMipLevel和MipMapLodBias (2005.6.28)
//
// 2.4.0
// 增加了DXTn的支持 (2005.3.6)
// 增加了1D/2D/3D/cube的支持 (2005.3.8)
//
// 2.3.0
// 增加了对浮点纹理格式的支持 (2005.1.25)
// 增加了CopyToMemory (2005.2.6)
//
// 修改记录
//////////////////////////////////////////////////////////////////////////////////

#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

#pragma once

#ifndef KLAYGE_CORE_SOURCE
#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>
#endif

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/ElementFormat.hpp>

#include <string>
#include <vector>
#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace KlayGE
{
	enum TextureMapAccess
	{
		TMA_Read_Only,
		TMA_Write_Only,
		TMA_Read_Write
	};

	// Abstract class representing a Texture resource.
	// @remarks
	// The actual concrete subclass which will exist for a texture
	// is dependent on the rendering system in use (Direct3D, OpenGL etc).
	// This class represents the commonalities, and is the one 'used'
	// by programmers even though the real implementation could be
	// different in reality.
	class KLAYGE_CORE_API Texture
	{
	public:
		// Enum identifying the texture type
		enum TextureType
		{
			// 1D texture, used in combination with 1D texture coordinates
			TT_1D,
			// 2D texture, used in combination with 2D texture coordinates
			TT_2D,
			// 3D texture, used in combination with 3D texture coordinates
			TT_3D,
			// cube map, used in combination with 3D texture coordinates
			TT_Cube
		};

		enum CubeFaces
		{
			CF_Positive_X = 0,
			CF_Negative_X = 1,
			CF_Positive_Y = 2,
			CF_Negative_Y = 3,
			CF_Positive_Z = 4,
			CF_Negative_Z = 5
		};

	public:
		class Mapper : boost::noncopyable
		{
			friend class Texture;

		public:
			Mapper(Texture& tex, int level, TextureMapAccess tma,
						uint32_t x_offset, uint32_t width)
				: tex_(tex),
					mapped_level_(level)
			{
				tex_.Map1D(level, tma, x_offset, width, data_);
				row_pitch_ = slice_pitch_ = width * tex.Bpp() / 8;
			}
			Mapper(Texture& tex, int level, TextureMapAccess tma,
						uint32_t x_offset, uint32_t y_offset,
						uint32_t width, uint32_t height)
				: tex_(tex),
					mapped_level_(level)
			{
				tex_.Map2D(level, tma, x_offset, y_offset, width, height, data_, row_pitch_);
				slice_pitch_ = row_pitch_ * height;
			}
			Mapper(Texture& tex, int level, TextureMapAccess tma,
						uint32_t x_offset, uint32_t y_offset, uint32_t z_offset,
						uint32_t width, uint32_t height, uint32_t depth)
				: tex_(tex),
					mapped_level_(level)
			{
				tex_.Map3D(level, tma, x_offset, y_offset, z_offset, width, height, depth, data_, row_pitch_, slice_pitch_);
			}
			Mapper(Texture& tex, CubeFaces face, int level, TextureMapAccess tma,
						uint32_t x_offset, uint32_t y_offset,
						uint32_t width, uint32_t height)
				: tex_(tex),
					mapped_face_(face),
					mapped_level_(level)
			{
				tex_.MapCube(face, level, tma, x_offset, y_offset, width, height, data_, row_pitch_);
				slice_pitch_ = row_pitch_ * height;
			}

			~Mapper()
			{
				switch (tex_.Type())
				{
				case TT_1D:
					tex_.Unmap1D(mapped_level_);
					break;

				case TT_2D:
					tex_.Unmap2D(mapped_level_);
					break;

				case TT_3D:
					tex_.Unmap3D(mapped_level_);
					break;

				case TT_Cube:
					tex_.UnmapCube(mapped_face_, mapped_level_);
					break;
				}
			}

			template <typename T>
			const T* Pointer() const
			{
				return static_cast<T*>(data_);
			}
			template <typename T>
			T* Pointer()
			{
				return static_cast<T*>(data_);
			}

			uint32_t RowPitch() const
			{
				return row_pitch_;
			}

			uint32_t SlicePitch() const
			{
				return slice_pitch_;
			}

		private:
			Texture& tex_;

			void* data_;
			uint32_t row_pitch_, slice_pitch_;

			CubeFaces mapped_face_;
			int mapped_level_;
		};

	public:
		explicit Texture(TextureType type, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);
		virtual ~Texture();

		static TexturePtr NullObject();

		// Gets the name of texture
		virtual std::wstring const & Name() const = 0;

		// Gets the number of mipmaps to be used for this texture.
		uint32_t NumMipMaps() const;
		// Gets the size of texture array
		uint32_t ArraySize() const;

		// Returns the width of the texture.
		virtual uint32_t Width(int level) const = 0;
		// Returns the height of the texture.
		virtual uint32_t Height(int level) const = 0;
		// Returns the depth of the texture (only for 3D texture).
		virtual uint32_t Depth(int level) const = 0;

		// Returns the bpp of the texture.
		uint32_t Bpp() const;
		// Returns the pixel format for the texture surface.
		ElementFormat Format() const;

		// Returns the texture type of the texture.
		TextureType Type() const;

		uint32_t SampleCount() const;
		uint32_t SampleQuality() const;

		uint32_t AccessHint() const;

		// Copies (and maybe scales to fit) the contents of this texture to another texture.
		virtual void CopyToTexture(Texture& target) = 0;
		virtual void CopyToTexture1D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_xOffset, uint32_t src_width, uint32_t src_xOffset) = 0;
		virtual void CopyToTexture2D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset) = 0;
		virtual void CopyToTexture3D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
			uint32_t dst_xOffset, uint32_t dst_yOffset, uint32_t dst_zOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_depth,
			uint32_t src_xOffset, uint32_t src_yOffset, uint32_t src_zOffset) = 0;
		virtual void CopyToTextureCube(Texture& target, CubeFaces face, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset) = 0;

		virtual void BuildMipSubLevels() = 0;

		virtual void Map1D(int level, TextureMapAccess tma,
			uint32_t x_offset, uint32_t width,
			void*& data) = 0;
		virtual void Map2D(int level, TextureMapAccess tma,
			uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height,
			void*& data, uint32_t& row_pitch) = 0;
		virtual void Map3D(int level, TextureMapAccess tma,
			uint32_t x_offset, uint32_t y_offset, uint32_t z_offset,
			uint32_t width, uint32_t height, uint32_t depth,
			void*& data, uint32_t& row_pitch, uint32_t& slice_pitch) = 0;
		virtual void MapCube(CubeFaces face, int level, TextureMapAccess tma,
			uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height,
			void*& data, uint32_t& row_pitch) = 0;

		virtual void Unmap1D(int level) = 0;
		virtual void Unmap2D(int level) = 0;
		virtual void Unmap3D(int level) = 0;
		virtual void UnmapCube(CubeFaces face, int level) = 0;

	protected:
		uint32_t		bpp_;

		uint32_t		numMipMaps_;
		uint32_t		array_size_;

		ElementFormat	format_;
		TextureType		type_;
		uint32_t		sample_count_, sample_quality_;
		uint32_t		access_hint_;
	};

	KLAYGE_CORE_API void LoadTexture(std::string const & tex_name, Texture::TextureType& type,
		uint32_t& width, uint32_t& height, uint32_t& depth, uint32_t& num_mipmaps, uint32_t& array_size,
		ElementFormat& format, std::vector<ElementInitData>& init_data, std::vector<uint8_t>& data_block);
	KLAYGE_CORE_API boost::function<TexturePtr()> LoadTexture(std::string const & tex_name, uint32_t access_hint);

	KLAYGE_CORE_API void SaveTexture(std::string const & tex_name, Texture::TextureType type,
		uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mipmaps, uint32_t array_size,
		ElementFormat format, std::vector<ElementInitData> const & init_data);
	KLAYGE_CORE_API void SaveTexture(TexturePtr texture, std::string const & tex_name);

	// 返回立方环境映射的lookat和up向量
	//////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline std::pair<Vector_T<T, 3>, Vector_T<T, 3> >
	CubeMapViewVector(Texture::CubeFaces face)
	{
		Vector_T<T, 3> look_dir;
		Vector_T<T, 3> up_dir;

		switch (face)
		{
		case Texture::CF_Positive_X:
			look_dir	= Vector_T<T, 3>(1, 0, 0);
			up_dir		= Vector_T<T, 3>(0, 1, 0);
			break;

		case Texture::CF_Negative_X:
			look_dir	= Vector_T<T, 3>(-1, 0, 0);
			up_dir		= Vector_T<T, 3>(0, 1, 0);
			break;

		case Texture::CF_Positive_Y:
			look_dir	= Vector_T<T, 3>(0, 1, 0);
			up_dir		= Vector_T<T, 3>(0, 0, -1);
			break;

		case Texture::CF_Negative_Y:
			look_dir	= Vector_T<T, 3>(0, -1, 0);
			up_dir		= Vector_T<T, 3>(0, 0, 1);
			break;

		case Texture::CF_Positive_Z:
			look_dir	= Vector_T<T, 3>(0, 0, 1);
			up_dir		= Vector_T<T, 3>(0, 1, 0);
			break;

		case Texture::CF_Negative_Z:
			look_dir	= Vector_T<T, 3>(0, 0, -1);
			up_dir		= Vector_T<T, 3>(0, 1, 0);
			break;
		}

		// 设置立方体环境映射的观察矩阵
		return std::make_pair(look_dir, up_dir);
	}
}

#endif			// _TEXTURE_HPP
