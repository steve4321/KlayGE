// DInputFactory.hpp
// KlayGE DirectInput输入引擎抽象工厂 头文件
// Ver 2.0.3
// 版权所有(C) 龚敏敏, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.3
// 改为template实现 (2004.3.4)
//
// 2.0.0
// 初次建立 (2003.8.30)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#ifndef _DINPUTFACTORY_HPP
#define _DINPUTFACTORY_HPP

#define KLAYGE_LIB_NAME KlayGE_InputEngine_DInput
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/InputFactory.hpp>

namespace KlayGE
{
	InputFactory& DInputFactoryInstance();
}

#endif			// _DINPUTFACTORY_HPP
