﻿#include "Interpreter.h"
#include <iostream>

#include <LBase/FCommon.h>

//#include YFM_Helper_Environment
//#include YFM_YSLib_Service_TextFile

using namespace leo;

#define NPL_TracePerform 1
#define NPL_TracePerformDetails 0

namespace scheme
{

#define NPL_NAME "NPL console"
#define NPL_VER "b30xx"
#define NPL_PLATFORM "[MinGW32]"
lconstexpr auto prompt("> ");
lconstexpr auto title(NPL_NAME" " NPL_VER" @ (" __DATE__", " __TIME__") "
	NPL_PLATFORM);

/// 519
namespace
{

/// 520
using namespace platform_ex::Windows;

/// 691
LB_NONNULL(3) void
PrintError(WConsole& wc, const LoggedEvent& e, const char* name = "Error")
{
	wc.UpdateForeColor(ErrorColor);
	LFL_TraceRaw(e.GetLevel(), "%s[%s]<%u>: %s", name, typeid(e).name(),
		unsigned(e.GetLevel()), e.what());
//	ExtractAndTrace(e, e.GetLevel());
}

} // unnamed namespace;


void
LogTree(const ValueNode& node, Logger::Level lv)
{
	std::ostringstream oss;

	PrintNode(oss, node, [](const ValueNode& node){
		return EscapeLiteral([&]() -> string{
			if(const auto p = AccessPtr<string>(node))
				return *p;
			if(const auto p = AccessPtr<bool>(node))
				return *p ? "[bool] #t" : "[bool] #f";

			const auto& v(node.Value);
			const auto& t(v.GetType());

			if(t != leo::type_id<void>())
				return leo::quote(string(t.name()), '[', ']');
			throw leo::bad_any_cast();
		}());
	});
	TraceDe(lv, "%s", oss.str().c_str());
}


Interpreter::Interpreter(Application& app,
	std::function<void(REPLContext&)> loader)
	: wc(), err_threshold(RecordLevel(0x10)), line(),
#if NPL_TracePerformDetails
	context(true)
#else
	context()
#endif
{
	using namespace std;
	using namespace platform_ex;

	wc.UpdateForeColor(TitleColor);
	cout << title << endl << "Initializing...";
	//p_env.reset(new Environment(app));
	loader(context);
	cout << "NPLC initialization OK!" << endl << endl;
	wc.UpdateForeColor(InfoColor);
	cout << "Type \"exit\" to exit,"
		" \"cls\" to clear screen, \"help\", \"about\", or \"license\""
		" for more information." << endl << endl;
}

void
Interpreter::HandleSignal(SSignal e)
{
	using namespace std;

	static lconstexpr auto not_impl("Sorry, not implemented: ");

	switch(e)
	{
	case SSignal::ClearScreen:
		wc.Clear();
		break;
	case SSignal::About:
		cout << not_impl << "About" << endl;
		break;
	case SSignal::Help:
		cout << not_impl << "Help" << endl;
		break;
	case SSignal::License:
		cout << "See license file of the YSLib project." << endl;
		break;
	default:
		LAssert(false, "Wrong command!");
	}
}

bool
Interpreter::Process()
{
	using namespace platform_ex;

	if(!line.empty())
	{
		wc.UpdateForeColor(SideEffectColor);
		try
		{
			line = DecodeArg(line);

			const auto res(context.Perform(line));

#if NPL_TracePerform
		//	wc.UpdateForeColor(InfoColor);
		//	cout << "Unrecognized reduced token list:" << endl;
			wc.UpdateForeColor(ReducedColor);
			LogTree(res);
#endif
		}
		catch(SSignal e)
		{
			if(e == SSignal::Exit)
				return {};
			wc.UpdateForeColor(SignalColor);
			HandleSignal(e);
		}
		CatchExpr(LSLException& e, PrintError(wc, e, "NPLException"))
		catch(LoggedEvent& e)
		{
			if(e.GetLevel() < err_threshold)
				throw;
			PrintError(wc, e);
		}
	}
	return true;
}

std::istream&
Interpreter::WaitForLine()
{
	return WaitForLine(std::cin, std::cout);
}
std::istream&
Interpreter::WaitForLine(std::istream& is, std::ostream& os)
{
	wc.UpdateForeColor(PromptColor);
	os << prompt;
	wc.UpdateForeColor(DefaultColor);
	return std::getline(is, line);
}

} // namespace NPL;
