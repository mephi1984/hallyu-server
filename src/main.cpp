#include "main.h"


#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/random.hpp"
#include "boost/program_options.hpp"

#include "http_server.h"
#include "socket_server.h"
#include "MySqlConnector.h"
#include "HangulProcessor.h"
#include "NumbersProcessor.h"


#include "LuaHelper.h"

//#include "zip.h"

//#include "ZipFile.h"

//int BasePort = 43333;
int BasePort = 80;

static int testLuaFunc(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	/* loop through each argument */
	for (i = 1; i <= n; i++)
	{
		/* total the arguments */
		sum += lua_tonumber(L, i);
	}

	/* push the average */
	lua_pushnumber(L, sum / n);

	/* push the sum */
	lua_pushnumber(L, sum);

	/* return the number of results */
	return 2;
}

int main(int argc, char *argv[])
{
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("foreground", "run in foreground (not as daemon)")
		;

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);

#ifdef _WIN32
	UINT oldcodepage = GetConsoleOutputCP();
	SetConsoleOutputCP(65001);
#endif


	//LH::ExecuteCode(SE::wstring_to_string(L"function f()\nttal = Syllable:new(\"딸\")\nstr = \"\"\nif (ttal:LastChamoExists()) then\nstr = str .. \"ttal:LastChamoExists\"\nend\nif (ttal:LastChamoExistsAndItIsNotL()) then\nstr = str .. \"ttal : LastChamoExistsAndItIsNotL\"\nend\nreturn str\nend"));
	//LH::ProcessString(L"먹고 싶어요");
	//LH::ProcessString(L"집은");
	//LH::ProcessString(L"바다와");
	//LH::ProcessString(L"자동차하고");
	//LH::ProcessString(L"너만을");
	//LH::ProcessString(L"시장에서");
	//LH::ProcessString(L"시장에");
	//LH::ProcessString(L"딸도");
	//LH::ProcessString(L"집까지");
	//LH::ProcessString(L"영원히");
	//LH::ProcessString(L"소개시켜");

	/*
	LH::ProcessString(L"바라보다");


	
	LH::ProcessString(L"와");

	LH::ProcessString(L"좋아해요");
	LH::ProcessString(L"좋아했어요");
	LH::ProcessString(L"왔어");
	LH::ProcessString(L"있었어요");

	LH::ProcessString(L"사랑할");
	*/
	//LH::ProcessString(L"못좋아했어요");
	/*
	LH::ProcessString(L"알겠습니다");

	LH::ProcessString(L"사랑하고");
	LH::ProcessString(L"사랑하니까");

	LH::ProcessString(L"사랑하는");

	LH::ProcessString(L"사랑하니까");
	
	
	LH::ProcessString(L"알니까");
	LH::ProcessString(L"알으니까");
	LH::ProcessString(L"먹으니까"); 
	LH::ProcessString(L"먹니까"); 
	*/

	//LH::ProcessString(L"안돼");
	/*
	LH::ProcessString(L"아세요");
	LH::ProcessString(L"알세요");
	LH::ProcessString(L"알으세요");
	*/

	//LH::ProcessString(L"알");
	//LH::ProcessString(L"알게");
	//LH::ProcessString(L"알때");

	//LH::ProcessString(L"아는데");

	//LH::ProcessString(L"알수록");

	//LH::ProcessString(L"압니다");

	//LH::ProcessString(L"찾았다");
	/*
	LH::ProcessString(L"알았어");
	LH::ProcessString(L"알았아");
	LH::ProcessString(L"알었어");
	LH::ProcessString(L"알었아");
	LH::ProcessString(L"알어");
	LH::ProcessString(L"알아");
	LH::ProcessString(L"먹어");
	LH::ProcessString(L"먹아");
	LH::ProcessString(L"먹았아");
	LH::ProcessString(L"먹았어");
	LH::ProcessString(L"먹었아");
	LH::ProcessString(L"먹었어");*/

	//LH::ProcessString(L"먹지 않아요");
	//LH::ProcessString(L"먹기도 했었어요");
	//LH::ProcessString(L"먹어 주세요");
	//LH::ProcessString(L"먹지 마세요");
	//LH::ProcessString(L"먹고 없어요");

	//LH::ProcessString(L"했었어요");

	//LH::ProcessString(L"주세요");

	//LH::ProcessString(L"주다");
	//LH::ProcessString(L"줘");
	//LH::ProcessString(L"주는");
	//LH::ProcessString(L"주고");



	LH::LuaHelper luaHelper;

	//std::vector<LH::TWordRecord> r1 = luaHelper.HangulDictionary.SearchWordByRussian(L"нед");

	//std::vector<LH::TWordRecord> r2 = luaHelper.HangulDictionary.SearchWordByRussian(L"них");

	//std::vector<LH::TWordRecord> r3 = luaHelper.HangulDictionary.SearchWordByRussian(L"пах");


	LH::HangulResult r1 = luaHelper.ProcessString(L"했었어요");
	LH::HangulResult r2 = luaHelper.ProcessString(L"먹지 않아요");

	LH::HangulResult r3 = luaHelper.ProcessString(L"백오십이");


	/*
	int errorp = 0;

	zip_t* zipFile = zip_open("C:\\Workplace\\Projects\\hallyu\\hallyu_server\\scripts\\purchase.zip", ZIP_CREATE, &errorp);


	zip_source_t* main_lua_source = zip_source_file(zipFile, "C:\\Workplace\\Projects\\hallyu\\hallyu_server\\scripts\\main.lua", 0, 54947);

	zip_int64_t rz2 = zip_file_add(zipFile, "main.lua", main_lua_source, ZIP_FL_ENC_UTF_8);

	//zip_source_free(main_lua_source);

	int cc = zip_close(zipFile);
	*/

	/*
	std::string s = "lolo\nddd\necho";
	std::vector<std::string> sarr;

	int n = s.find("\n");

	while (n != std::string::npos)
	{
		sarr.push_back(s.substr(0, n));
		s.erase(0, n + 1);
		n = s.find("\n");
	}*/

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 1;
	}

	if (vm.count("foreground"))
	{
		std::cout <<"Running in foreground" << std::endl;
	}
	else
	{
#ifndef _WIN32
		std::cout << "Running in background" << std::endl;
		//daemonize();
#endif
	}




#ifdef _WIN32
	setlocale(LC_ALL, "RUSSIAN");
#endif

	SE::WriteToLog("Server started!");


	//std::cout << CalcVkCheckCode("677718");

	//THangulProcessor HangulProcessor;

	//13103 fail

	//std::wstring n = UintToChineseNumberString(13103);
	/*
	n = UintToKoreanNumberString(73);

	n = UintToKoreanNumberString(123);

	n = UintToKoreanNumberString(3452);

	n = UintToKoreanNumberString(443);

	std::wstring numw = L"삼백구십이";

	TChineseNumber rnum;

	bool rn = RecognizeChineseNumber(numw, rnum);

	std::cout << rnum.number << std::endl;
	*/
	

	//std::string verbose;
	//std::set<std::wstring> outLessons;




	//bool rb = HangulProcessor.RecognizeAndVerboseWords({ L"겠어요" }, verbose, outLessons);

	//bool rb = HangulProcessor.RecognizeAndVerboseWords({ L"아니까" }, verbose, outLessons);
	
	//bool rb = HangulProcessor.RecognizeAndVerboseWords({ L"됐다" }, verbose, outLessons);

	//bool rb = HangulProcessor.RecognizeAndVerboseWords({ L"책들을만도" }, verbose, outLessons);

	/*
	std::locale::global(std::locale(""));

	if (rb)
	{
		std::cout << verbose << std::endl;
	}*/
	
	SE::WriteToLog("Server start step 1=");

	TMySqlConnector MySqlConnector;

	try
	{

		//SE::TServerSocket ServerSocket(BasePort);
	
		
		

		SE::WriteToLog("Server start step 2=");

		//std::shared_ptr<SE::TServerSocket> testServer(new SE::TServerSocket(43444));

		//SE::WriteToLog("Server start step 2-1=");

		{

			SE::WriteToLog("Server start step 2-2=");

			boost::asio::io_service io_service;

			SE::WriteToLog("Server start step 2-3=");

			TImageDownloader TestImageDownloader(io_service);

			SE::WriteToLog("Server start step 2-4=");
		}

		SE::WriteToLog("Server start step 2-5=");

		//std::shared_ptr<THallyuSocketServer> Server(new THallyuSocketServer(BasePort, MySqlConnector, luaHelper));

		std::shared_ptr<http::server::THallyuHttpServer> Server(new http::server::THallyuHttpServer(BasePort, luaHelper));

		SE::WriteToLog("Server start step 3=");

		Server->JoinServiceThread();

	}
	catch (const std::exception& e)
	{
		SE::WriteToLog(std::string("std::exception in main ") + e.what());
	}

	return 0;
}

