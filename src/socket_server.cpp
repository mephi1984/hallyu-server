#include "socket_server.h"

#include "boost/random.hpp"
#include "boost/thread/thread.hpp"
#include "boost/thread/mutex.hpp"

#include "NumbersProcessor.h"

//#include "ZipFile.h"

#include <fstream>
#include "md5.h"

#include "inner_base64.h"

//For SHA1
#include <openssl/sha.h>


const int CONST_DICTIONARY_CURRENT_VERSION = 1;


std::string CalcVkCheckCode(const std::string& vkLogin)
{
	const std::string prefix = "db5543e6bf25f273f543e164955d0511cef6787b";

	std::string str = prefix + vkLogin;

	std::vector<unsigned char> out;
	out.resize(20);

	unsigned char* byteResult = SHA1(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), &out[0]);

	const unsigned char* uOut = reinterpret_cast<const unsigned char*>(&out[0]);

	std::string result = B::base64_encode(&out[0], out.size());
	
	return result;

}


std::string CalcPurchaseCheckCode(int purchaseId, const std::string& username)
{

	std::string str = (boost::lexical_cast<std::string>(purchaseId)+"yeogi isseo deo deo butak halkke deo deo jalhae julkke deo deo ajigeun mot bonaenikka" + username);

	std::vector<unsigned char> out;
	out.resize(20);

	unsigned char* byteResult = SHA1(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), &out[0]);

	const unsigned char* uOut = reinterpret_cast<const unsigned char*>(&out[0]);

	std::string result = B::base64_encode(&out[0], out.size());

	return result;

}


#ifdef USE_MYSQL

void TUser::InnerStartReadBinary(boost::property_tree::ptree binaryTree)
{
	int size = binaryTree.get<int>("_size", 0);

	Data.resize(size);

	boost::asio::async_read(Socket, boost::asio::buffer(Data), std::bind(&TUser::HandleReadBinaryData, shared_from_this(), binaryTree, std::placeholders::_1));
}

#endif

void TUser::BeforeStartRead()
{
	Address = Socket.lowest_layer().remote_endpoint().address().to_string();

	DataReadSignalMap.AddSlot("RequestWordTranslation", std::bind(&TUser::OnReceive_RequestWordTranslation, this, std::placeholders::_1));

	DataReadSignalMap.AddSlot("RequestCard", std::bind(&TUser::OnReceive_RequestCard, this, std::placeholders::_1));
	DataReadSignalMap.AddSlot("RequestChineseNumberRecognize", std::bind(&TUser::OnReceive_RequestChineseNumberRecognize, this, std::placeholders::_1));

#ifdef USE_MYSQL

		DataReadSignalMap.AddSlot("GetFeed", std::bind(&TUser::OnReceive_GetFeed, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("GetFeedList", std::bind(&TUser::OnReceive_GetFeedList, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("RegisterViaVk", std::bind(&TUser::OnReceive_RegisterViaVk, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("GetUser", std::bind(&TUser::OnReceive_GetUser, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("AddPost", std::bind(&TUser::OnReceive_AddPost, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("AddComment", std::bind(&TUser::OnReceive_AddComment, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("GetComments", std::bind(&TUser::OnReceive_GetComments, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("GetComicsSeries", std::bind(&TUser::OnReceive_GetComicsSeries, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("GetComicsEpisodes", std::bind(&TUser::OnReceive_GetComicsEpisodes, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("GetComicsPictures", std::bind(&TUser::OnReceive_GetComicsPictures, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("GetPurchaseList", std::bind(&TUser::OnReceive_GetPurchaseList, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("Purchase", std::bind(&TUser::OnReceive_Purchase, this, std::placeholders::_1));
		DataReadSignalMap.AddSlot("DownloadPurchase", std::bind(&TUser::OnReceive_DownloadPurchase, this, std::placeholders::_1));

#endif
}

void TUser::StartRead()
{
	SE::ErrorToLog("Data reader start read: " + boost::lexical_cast<std::string>(this));
	boost::asio::async_read(Socket, boost::asio::buffer(&DataSize, 4), std::bind(&TUser::HandleReadDataSize, shared_from_this(), std::placeholders::_1));
}


void TUser::HandleReadDataSize(const boost::system::error_code& error)
{
	if (error)
	{
		SE::WriteToLog("Error in inner HandleReadDataSize: " + boost::lexical_cast<std::string>(this));
		return;
	}


	if (DataSize > 65536 || DataSize <= 0)
	{
		// len>65536 is something unbelievable. Prevent this just in case;
		SE::WriteToLog("Error in inner HandleReadDataSize, DataSize is too long or too small: " + boost::lexical_cast<std::string>(this));
		return;
	}

	Data.resize(DataSize);

	boost::asio::async_read(Socket, boost::asio::buffer(Data), std::bind(&TUser::HandleReadData, shared_from_this(), std::placeholders::_1));

}

void TUser::HandleReadData(const boost::system::error_code& error)
{
	if (error)
	{
		SE::WriteToLog("Error in inner HandleReadData: " + boost::lexical_cast<std::string>(this));
		return;
	}

	try
	{
		//Xperimental - Might be optimized a lot:

		std::string xmlCode = std::string(&Data[0], &Data[0] + Data.size());

		std::stringstream stream(xmlCode);

		boost::property_tree::ptree propertyTree;

		boost::property_tree::read_xml(stream, propertyTree);

		bool nextIsBinary = false;
		boost::property_tree::ptree binaryTree;

		BOOST_FOREACH(auto i, propertyTree)
		{
			if (i.first == "_binary")
			{
				binaryTree = i.second;
				nextIsBinary = true;

			}
			else
				if (DataReadSignalMap.SignalExists(i.first))
				{
					DataReadSignalMap.EmitSignal(i.first, i.second);
				}
		}


#if 0
		if (nextIsBinary)
		{
			InnerStartReadBinary(binaryTree);
		}
		else
		{
			StartRead();
		}
#else
		StartRead();
#endif


	}
	catch (boost::property_tree::ptree_error)
	{
		SE::WriteToLog("Error in inner HandleReadData: ptree_error exception caught: " + boost::lexical_cast<std::string>(this));
	}
}


#ifdef USE_MYSQL

void TUser::HandleReadBinaryData(boost::property_tree::ptree binaryTree, const boost::system::error_code& error)
{
	if (error)
	{
		SE::WriteToLog("Error in inner HandleReadBinaryData: " + boost::lexical_cast<std::string>(this));
		return;
	}

	try
	{
		ProcessBinaryData(binaryTree, Data);

		StartRead();
	}
	catch (boost::property_tree::ptree_error)
	{
		SE::WriteToLog("Error in inner HandleReadBinaryData: ptree_error exception caught: " + boost::lexical_cast<std::string>(this));
	}

}

#endif

void TUser::SendPropertyTree(boost::property_tree::ptree pTree)
{
	//Xperimental -- need to optimize this
	try
	{

		std::stringstream o_stream;

		boost::property_tree::write_xml(o_stream, pTree);

		std::string data = o_stream.str();

		int len = data.size();

		boost::shared_array<char> dataToSend(new char[len + 4]);

		memcpy(&dataToSend[0], &len, 4);

		memcpy(&dataToSend[4], &data[0], len);

		auto sharedThis = shared_from_this(); // to keep connection


		boost::asio::async_write(Socket, boost::asio::buffer(&dataToSend[0], len + 4),
			[dataToSend, sharedThis](boost::system::error_code ec, std::size_t)
		{
			if (ec)
			{
				SE::WriteToLog("Error in inner SendPropertyTree");
			}
		}
		);
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Error in SendPropertyTree");
	}
}

void TUser::SendBinary(const std::vector<char>& binaryData)
{
	//Xperimental -- need to optimize this
	try
	{

		std::string text = "<_binary><_size>" + boost::lexical_cast<std::string>(binaryData.size()) + "</_size></_binary>";

		int xmlLen = text.size();

		int totalLen = binaryData.size() + xmlLen + 4;

		boost::shared_array<char> dataToSend(new char[totalLen]);

		memcpy(&dataToSend[0], &xmlLen, 4);

		memcpy(&dataToSend[4], &text[0], xmlLen);

		if (binaryData.size() > 0)
		{
			memcpy(&dataToSend[4 + xmlLen], &binaryData[0], binaryData.size());
		}

		auto sharedThis = shared_from_this(); // to keep connection

		boost::asio::async_write(Socket, boost::asio::buffer(&dataToSend[0], totalLen),
			[dataToSend, sharedThis](boost::system::error_code ec, std::size_t)
		{
			if (ec)
			{
				SE::WriteToLog("Error in inner SendBinary");
			}
		}
		);
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Error in SendBinary");
	}
}


int TUser::counter = 0;

TUser::TUser(boost::asio::io_service& ioService, boost::asio::ip::tcp::socket socket, THallyuSocketServer& server)
	: HallyuSocketServer(server)
	, Socket(std::move(socket))
	, IoService(ioService)
{
	selfCounter = counter;
	counter++;
	SE::WriteToLog("TUser::TUser " + boost::lexical_cast<std::string>(selfCounter)+" : " + boost::lexical_cast<std::string>(this));
	
}

TUser::~TUser()
{
	SE::WriteToLog("TUser::~TUser " + boost::lexical_cast<std::string>(selfCounter)+" : " + boost::lexical_cast<std::string>(this));
}

void TUser::OnReceive_RequestWordTranslation(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_RequestWordTranslation begin");
		std::string word = propertyTree.get<std::string>("");

		std::string wordSymbols;
		for (auto i : word)
		{
			wordSymbols += boost::lexical_cast<std::string>((int)i) + " ";
		}
		SE::WriteToLog("OnReceive_RequestWordTranslation word: " + wordSymbols);
		SE::WriteToLog("OnReceive_RequestWordTranslation word: " + word);

		Send_OnRequestWordTranslation(word);

		SE::WriteToLog("OnReceive_RequestWordTranslation end");
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_RequestWordTranslation");
	}
}

void TUser::OnReceive_RequestCard(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_RequestCard begin");
		size_t card = propertyTree.get<size_t>("");

		Send_OnRequestCard(card);

		SE::WriteToLog("OnReceive_RequestCard: " + boost::lexical_cast<std::string>(card));
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_RequestCard");
	}
}

void TUser::OnReceive_RequestChineseNumberRecognize(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_RequestChineseNumberRecognize begin");
		int number = propertyTree.get<int>("");
		Send_OnRequestChineseNumberRecognize(number);

		SE::WriteToLog("OnReceive_RequestChineseNumberRecognize: " + boost::lexical_cast<std::string>(number));
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_RequestChineseNumberRecognize");
	}
}


#ifdef USE_MYSQL


void TUser::OnReceive_GetFeed(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_GetFeed begin");

		int feedId = propertyTree.get<int>("", -1);

		if (feedId == -1)
		{
			//New style since app version 7
			int feedId = propertyTree.get<int>("feedId");
			int startingFromPostId = propertyTree.get<int>("startingFromPostId");
			int count = propertyTree.get<int>("count");
			Send_OnGetFeed(feedId, startingFromPostId, count);
		}
		else
		{
			//Old style app version 7 or earlier
			Send_OnGetFeed(feedId, -1, 10);
		}
		SE::WriteToLog("OnReceive_GetFeed: " + boost::lexical_cast<int>(feedId));
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_GetFeed");
	}
}



void TUser::OnReceive_GetFeedList(boost::property_tree::ptree propertyTree)
{
	try
	{
		Send_OnGetFeedList();
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_GetFeedList");
	}
}


void TUser::OnReceive_RegisterViaVk(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_RegisterViaVk begin");
		std::string vkLogin = propertyTree.get<std::string>("vkLogin");
		std::string vkCheckCode = propertyTree.get<std::string>("vkCheckCode");
		std::string firstName = propertyTree.get<std::string>("firstName");
		std::string lastName = propertyTree.get<std::string>("lastName");
		std::string photo200Url = propertyTree.get<std::string>("photo200Url");


		if (vkCheckCode == CalcVkCheckCode(vkLogin))
		{

			SE::WriteToLog("OnReceive_RegisterViaVk, vkCheckCode == CalcVkCheckCode(vkLogin) succeed");

			SE::WriteToLog("Download photo begins");
			//Xperimental! This might be potentially slow - function downloads photo, and only then function returns responce
			HallyuSocketServer.RegisterVkUser(vkLogin, firstName, lastName, photo200Url);
			SE::WriteToLog("Download photo ends");

			TUserListElement user = HallyuSocketServer.GetUserByVkLogin(vkLogin);

			Send_OnRegisterViaVk(user.login);

			SE::WriteToLog("OnReceive_RegisterViaVk, vkLogin:" + vkLogin + " firstName: " + firstName + " lastName: " + lastName + " photo200Url: " + photo200Url);

		}
		else
		{
			SE::WriteToLog("OnReceive_RegisterViaVk, vkCheckCode == CalcVkCheckCode(vkLogin) failed");
			//Send_OnRegisterViaVkFailed();
		}
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_RegisterViaVk");
	}
}


void TUser::OnReceive_RegisterViaVk_Api2(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_RegisterViaVk begin");
		std::string vkLogin = propertyTree.get<std::string>("vkLogin");
		std::string firstName = propertyTree.get<std::string>("firstName");
		std::string lastName = propertyTree.get<std::string>("lastName");
		std::string photo200Url = propertyTree.get<std::string>("photo200Url");

		SE::WriteToLog("Download photo begins");
		//Xperimental! This might be potentially slow - function downloads photo, and only then function returns responce
		HallyuSocketServer.RegisterVkUser(vkLogin, firstName, lastName, photo200Url);
		SE::WriteToLog("Download photo ends");

		TUserListElement user = HallyuSocketServer.GetUserByVkLogin(vkLogin);

		Send_OnRegisterViaVk(user.login);

		SE::WriteToLog("OnReceive_RegisterViaVk, vkLogin:" + vkLogin + " firstName: " + firstName + " lastName: " + lastName + " photo200Url: " + photo200Url);

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_RegisterViaVk");
	}
}


void TUser::OnReceive_GetUser(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_GetUser begin");
		//int number = propertyTree.get<int>("");
		std::string username = propertyTree.get<std::string>("");

		Send_OnGetUser(username);

		SE::WriteToLog("OnReceive_GetUser: " + username);
	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_RequestCard");
	}
}

void TUser::OnReceive_AddComment(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_AddComment begin");
		std::string username = propertyTree.get<std::string>("Username");
		int postId = propertyTree.get<int>("PostId");
		std::string commentText = propertyTree.get<std::string>("CommentText");

		HallyuSocketServer.AddComment(username, postId, commentText);

		Send_OnAddComment();

		SE::WriteToLog("OnReceive_AddComment: " + username + " " + boost::lexical_cast<std::string>(postId)+" " + commentText);


	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_AddComment");
	}
}

void TUser::OnReceive_GetComments(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_GetComments begin");
		int postId = propertyTree.get<int>("PostId");

		Send_OnGetComments(postId);

		SE::WriteToLog("OnReceive_GetComments: " + boost::lexical_cast<std::string>(postId));

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_GetComments");
	}
}


void TUser::OnReceive_AddPost(boost::property_tree::ptree propertyTree)
{
	//void AddPost(const std::string& vkLogin, int feedId, const std::string& postText);
	try
	{
		SE::WriteToLog("OnReceive_AddPost begin");
		std::string username = propertyTree.get<std::string>("Username");
		int feedId = propertyTree.get<int>("FeedId");
		std::string postHeader = propertyTree.get<std::string>("PostHeader");
		std::string postText = propertyTree.get<std::string>("PostText");

		HallyuSocketServer.AddPost(username, feedId, postHeader, postText);

		Send_OnAddPost();

		SE::WriteToLog("OnReceive_AddPost: " + username + " " + boost::lexical_cast<std::string>(feedId)+" " + postHeader + " " + postText);

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_AddPost");
	}
}

void TUser::OnReceive_GetComicsSeries(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_GetComicsSeries begin");
		//int postId = propertyTree.get<int>("PostId");

		Send_OnGetComicsSeries();

		SE::WriteToLog("OnReceive_GetComicsSeries");

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_GetComicsSeries");
	}
}


void TUser::OnReceive_GetComicsEpisodes(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_GetComicsEpisodes begin");
		int seriesId = propertyTree.get<int>("SeriesId");

		Send_OnGetComicsEpisodes(seriesId);

		SE::WriteToLog("OnReceive_GetComicsEpisodes: " + boost::lexical_cast<std::string>(seriesId));

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_GetComicsEpisodes");
	}
}

void TUser::OnReceive_GetComicsPictures(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("Send_OnGetComicsPictures begin");
		int episodeId = propertyTree.get<int>("EpisodeId");

		Send_OnGetComicsPictures(episodeId);

		SE::WriteToLog("Send_OnGetComicsPictures: " + boost::lexical_cast<std::string>(episodeId));

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::Send_OnGetComicsPictures");
	}
}

void TUser::OnReceive_GetPurchaseList(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_GetPurchaseList begin");

		Send_OnGetPurchaseList();

		SE::WriteToLog("OnReceive_GetPurchaseList end");

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_GetPurchaseList");
	}
}

void TUser::OnReceive_Purchase(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_Purchase begin");
		int purchaseId = propertyTree.get<int>("purchaseId");
		std::string username = propertyTree.get<std::string>("username");
		std::string purchaseCode = propertyTree.get<std::string>("purchaseCode");

		Send_OnPurchase(purchaseId, username, purchaseCode);

		SE::WriteToLog("OnReceive_Purchase: " + boost::lexical_cast<std::string>(purchaseId)+" " + username + " " + purchaseCode);

	}
	catch (std::exception e)
	{
		SE::WriteToLog("Exception in TUser::OnReceive_Purchase");
	}
}

void TUser::OnReceive_DownloadPurchase(boost::property_tree::ptree propertyTree)
{
	try
	{
		SE::WriteToLog("OnReceive_DownloadPurchase begin");
		int purchaseId = propertyTree.get<int>("PurchaseId");
		std::string username = propertyTree.get<std::string>("Username");
		std::string purchaseCode = propertyTree.get<std::string>("PurchaseCode");

		Send_OnDownloadPurchase(purchaseId, username, purchaseCode);

		SE::WriteToLog("OnReceive_DownloadPurchase: " + boost::lexical_cast<std::string>(purchaseId)+" " + username + " " + purchaseCode);

	}
	catch (std::exception e)
	{
		SE::WriteToLog(std::string("Exception in TUser::OnReceive_DownloadPurchase, ") + e.what());
	}
}



void TUser::ProcessBinaryData(boost::property_tree::ptree propertyTree, std::vector<char> binary)
{
	boost::property_tree::ptree addPostTree = propertyTree.get_child("AddPost");

	std::string username = addPostTree.get<std::string>("Username");
	int feedId = addPostTree.get<int>("FeedId");
	std::string postHeader = addPostTree.get<std::string>("PostHeader");
	std::string postText = addPostTree.get<std::string>("PostText");

	if (username != "")
	{

		TUserListElement user = HallyuSocketServer.GetUserByUserLogin(username);

		std::string fileName = user.login + "-" + boost::lexical_cast<std::string>(user.postCount + 1) + ".jpg";


		std::string filePath = TImageDownloader::getFilePath() + "uploaded/";

		try
		{
			std::ofstream f(filePath + fileName, std::ofstream::binary);

			if (f.is_open())
			{
				f.write(&binary[0], binary.size());
			}

			f.close();
		}
		catch (std::exception e)
		{
			SE::WriteToLog("Exception in TUser::ProcessBinaryData");
			fileName = "";
		}

		if (fileName != "")
		{
			MD5 md5;

			std::string imageHash = md5.digestFile((filePath + fileName).c_str());

			HallyuSocketServer.AddPostWithImage(username, feedId, postHeader, postText, "uploaded/" + fileName, imageHash);

			Send_OnAddPost();
		}

	}


}

#endif


void TUser::Send_OnRequestWordTranslation(std::string wordToTranslate)
{
	boost::property_tree::ptree p;

	//boost::trim(wordToTranslate);

	std::wstring wWordToTranslate = SE::string_to_wstring(wordToTranslate);

	boost::trim(wWordToTranslate);

	std::string verboseResult;

	std::set<std::wstring> lessonSet;

	if (wWordToTranslate.size() > 0)
	{

		//std::vector<std::wstring> wWordToTranslateArr;

		//boost::split(wWordToTranslateArr, wWordToTranslate, boost::is_any_of(L" "), boost::token_compress_on);

		bool hangulWord = true;

		for (wchar_t& wc : wWordToTranslate)
		{
			if (!((wc >= 44032 && wc <= 55203) || (wc == L' ')))
			{
				hangulWord = false;
			}
		}

		if (hangulWord)
		{
			//HallyuSocketServer.HangulProcessor.RecognizeAndVerboseWords(wWordToTranslateArr, verboseResult, lessonSet);



			LH::HangulResult result = HallyuSocketServer.luaHelper.ProcessString(wWordToTranslate);

			for (int i = 0; i < result.resultTable.size(); i++)
			{
				for (int j = 0; j < result.resultTable[i].size(); j++)
				{
					for (int k = 0; k < result.resultTable[i][j].dictStruct.words.size(); k++)
					{
						verboseResult += "\"" + SE::wstring_to_string(result.resultTable[i][j].dictStruct.words[k]) + "\" ";
					}

					verboseResult += "- ";

					verboseResult += SE::wstring_to_string(result.resultTable[i][j].verbose) + "\n";

					for (auto& lesson : result.resultTable[i][j].lessons)
					{
						lessonSet.insert(lesson);
					}

				}
			}

			for (int i = 0; i < result.complexVerbResultArr.size(); i++)
			{
				verboseResult += SE::wstring_to_string(result.complexVerbResultArr[i].verbose) + "\n";

				for (auto& lesson : result.complexVerbResultArr[i].lessons)
				{
					lessonSet.insert(lesson);
				}
			}

		}
		else
		{
			verboseResult = "Request contains non-hangul characters";
		}
	}
	else
	{
		verboseResult = "Request is empty";
	}

	SE::WriteToLog("Verbose result size : " + boost::lexical_cast<std::string>(verboseResult.size()));


	//p.put("OnRequestWordTranslation", verboseResult); // For andoid app version 5 or lower

	p.put("OnRequestWordTranslation.Verbose", verboseResult);

	boost::property_tree::ptree lesson_tree;

	for (const std::wstring& lesson : lessonSet)
	{
		lesson_tree.add("Lesson", SE::wstring_to_string(lesson));
	}
	p.add_child("OnRequestWordTranslation.LessonList", lesson_tree);

	SendPropertyTree(p);
}



void TUser::Send_OnRequestCard(size_t wordCount)
{

	//Xperimental
	boost::property_tree::ptree p;

	std::wstring translation;
	std::wstring word0;
	std::wstring word1;
	std::wstring word2;
	std::wstring word3;

	size_t correctAnswer;

	HallyuSocketServer.luaHelper.HangulDictionary.GetRandomWord(wordCount, translation, word0, word1, word2, word3, correctAnswer);

	p.put("OnRequestCard.Translation", SE::wstring_to_string(translation));
	p.put("OnRequestCard.Word0", SE::wstring_to_string(word0));
	p.put("OnRequestCard.Word1", SE::wstring_to_string(word1));
	p.put("OnRequestCard.Word2", SE::wstring_to_string(word2));
	p.put("OnRequestCard.Word3", SE::wstring_to_string(word3));
	p.put("OnRequestCard.CorrectWord", correctAnswer);

	SendPropertyTree(p);
}

void TUser::Send_OnRequestChineseNumberRecognize(int maxDigits)
{

	std::wstring numberStr;
	unsigned long long number;

	HallyuSocketServer.luaHelper.HangulDictionary.GetRandomChineseNumber(maxDigits, number, numberStr);

	boost::property_tree::ptree p;

	p.put("OnRequestChineseNumberRecognize.number", number);
	p.put("OnRequestChineseNumberRecognize.numberStr", SE::wstring_to_string(numberStr));


	SendPropertyTree(p);


}


#ifdef USE_MYSQL


void TUser::Send_OnGetFeed(int feedId, int startingFromPostId, int count)
{

	boost::property_tree::ptree p;

	p.put("OnGetFeed", "");

	TFeedListElement feedListElement = HallyuSocketServer.GetFeedListElement(feedId);
	std::vector<TPostListElement> feedElementVec = HallyuSocketServer.GetPostList(feedId, startingFromPostId, count);

	p.put("OnGetFeed.FeedListElement.Name", feedListElement.Name);
	p.put("OnGetFeed.FeedListElement.CommentMode", feedListElement.CommentMode);
	p.put("OnGetFeed.FeedListElement.PostMode", feedListElement.PostMode);

	for (size_t i = 0; i < feedElementVec.size(); i++)
	{
		boost::property_tree::ptree feed_element_tree;

		feed_element_tree.put("Id", feedElementVec[i].Id);
		feed_element_tree.put("Header", feedElementVec[i].Header);
		feed_element_tree.put("Text", feedElementVec[i].Text);
		feed_element_tree.put("ImageUrl", feedElementVec[i].ImageUrl);
		feed_element_tree.put("ImageHash", feedElementVec[i].ImageHash);
		feed_element_tree.put("Username", feedElementVec[i].Username);
		feed_element_tree.put("Date", feedElementVec[i].Date);

		p.add_child("OnGetFeed.FeedElements.FeedElement", feed_element_tree);
	}

	SendPropertyTree(p);
}

void TUser::Send_OnGetFeedList()
{
	boost::property_tree::ptree p;

	std::vector<TFeedListElement> feedListElementArr = HallyuSocketServer.GetFeedListElementArr();

	for (size_t i = 0; i < feedListElementArr.size(); i++)
	{
		boost::property_tree::ptree element_tree;

		element_tree.put("Id", feedListElementArr[i].Id);
		element_tree.put("Name", feedListElementArr[i].Name);
		element_tree.put("CommentMode", feedListElementArr[i].CommentMode);
		element_tree.put("PostMode", feedListElementArr[i].PostMode);

		p.add_child("OnGetFeedList.FeedListElements.FeedListElement", element_tree);
	}

	SendPropertyTree(p);
}



void TUser::Send_OnRegisterViaVk(const std::string& username)
{
	boost::property_tree::ptree p;

	p.put("OnRegisterViaVk", username);

	//p.add_child("OnRegisterViaVk.purchases", CreatePurchasesTree(username));

	SendPropertyTree(p);
}



void TUser::Send_OnGetUser(const std::string& username)
{
	TUserListElement userListElement = HallyuSocketServer.GetUserByUserLogin(username);

	boost::property_tree::ptree p;

	p.put("OnGetUser.postCount", userListElement.postCount);
	p.put("OnGetUser.firstName", userListElement.firstName);
	p.put("OnGetUser.lastName", userListElement.lastName);
	p.put("OnGetUser.photoUrl", userListElement.photoUrl);
	p.put("OnGetUser.photoHash", userListElement.photoHash);

	boost::property_tree::ptree purchases_tree;

	std::vector<std::pair<int, int>> purchaseList = HallyuSocketServer.GetPurchaseListWithVersion(username);

	for (auto& purchaseIdAndHash : purchaseList)
	{
		boost::property_tree::ptree purchaseTree;

		purchaseTree.put("id", purchaseIdAndHash.first);
		purchaseTree.put("version", purchaseIdAndHash.second);

		purchases_tree.add_child("purchase", purchaseTree);
	}

	p.add_child("OnGetUser.purchases", purchases_tree);

	//p.put("OnGetUser.purchaseVersion", 1);

	SendPropertyTree(p);

}

void TUser::Send_OnAddComment()
{
	boost::property_tree::ptree p;

	p.put("OnAddComment", "Succeed"); //Xperimental -- need proper feedback

	SendPropertyTree(p);

}

void TUser::Send_OnGetComments(int postId)
{
	boost::property_tree::ptree p;

	p.put("OnGetComments", "");

	std::vector<TCommentStruct> comments = HallyuSocketServer.GetComments(postId);

	for (size_t i = 0; i < comments.size(); i++)
	{
		boost::property_tree::ptree feed_element_tree;
		feed_element_tree.put("commentId", comments[i].CommentId);
		feed_element_tree.put("text", comments[i].CommentText);
		feed_element_tree.put("imageUrl", comments[i].ImageUrl);
		feed_element_tree.put("imageHash", comments[i].ImageHash);
		feed_element_tree.put("authorName", comments[i].AuthorName);
		feed_element_tree.put("authorPhotoUrl", comments[i].AuthorPhotoUrl);
		feed_element_tree.put("authorPhotoHash", comments[i].AuthorPhotoHash);
		feed_element_tree.put("date", comments[i].Date);

		p.add_child("OnGetComments.Comments.Comment", feed_element_tree);
	}

	SendPropertyTree(p);
}

void TUser::Send_OnAddPost()
{
	boost::property_tree::ptree p;

	p.put("OnAddPost", "Succeed"); //Xperimental -- need proper feedback

	SendPropertyTree(p);

}


void TUser::Send_OnGetComicsSeries()
{
	boost::property_tree::ptree p;

	p.put("OnGetComicsSeries", "");

	std::vector<TComicsSeriesListElement> series = HallyuSocketServer.GetComicsSeries();

	for (size_t i = 0; i < series.size(); i++)
	{
		boost::property_tree::ptree element_tree;
		element_tree.put("seriesId", series[i].seriesId);
		element_tree.put("translatedTitle", series[i].translatedTitle);
		element_tree.put("originalTitle", series[i].originalTitle);
		element_tree.put("description", series[i].description);
		element_tree.put("episodesCount", series[i].episodesCount);
		element_tree.put("translatedImageUrl", series[i].translatedImageUrl);
		element_tree.put("translatedImageHash", series[i].translatedImageHash);
		element_tree.put("originalImageUrl", series[i].originalImageUrl);
		element_tree.put("originalImageHash", series[i].originalImageHash);

		p.add_child("OnGetComicsSeries.Series.Ser", element_tree);
	}

	SendPropertyTree(p);
}

void TUser::Send_OnGetComicsEpisodes(int seriesId)
{
	boost::property_tree::ptree p;

	p.put("OnGetComicsEpisodes", "");

	std::vector<TComicsEpisodeListElement> episodes = HallyuSocketServer.GetComicsEpisodes(seriesId);

	for (size_t i = 0; i < episodes.size(); i++)
	{
		boost::property_tree::ptree element_tree;

		element_tree.put("episodeId", episodes[i].episodeId);
		element_tree.put("seriesId", episodes[i].seriesId);
		element_tree.put("title", episodes[i].title);
		element_tree.put("description", episodes[i].description);
		element_tree.put("position", episodes[i].position);
		element_tree.put("picturesCount", episodes[i].picturesCount);

		p.add_child("OnGetComicsEpisodes.Episodes.Episode", element_tree);
	}

	SendPropertyTree(p);
}


void TUser::Send_OnGetComicsPictures(int episodeId)
{
	boost::property_tree::ptree p;

	p.put("OnGetComicsPictures", "");

	std::vector<TComicsPictureListElement> pictures = HallyuSocketServer.GetComicsPictures(episodeId);

	for (size_t i = 0; i < pictures.size(); i++)
	{
		boost::property_tree::ptree element_tree;
		element_tree.put("pictureId", pictures[i].pictureId);
		element_tree.put("episodeId", pictures[i].episodeId);
		element_tree.put("position", pictures[i].position);
		element_tree.put("translatedImageUrl", pictures[i].translatedImageUrl);
		element_tree.put("translatedImageHash", pictures[i].translatedImageHash);
		element_tree.put("originalImageUrl", pictures[i].originalImageUrl);
		element_tree.put("originalImageHash", pictures[i].originalImageHash);

		p.add_child("OnGetComicsPictures.Pictures.Picture", element_tree);
	}

	SendPropertyTree(p);
}

void TUser::Send_OnGetPurchaseList()
{
	boost::property_tree::ptree p;

	p.put("OnGetPurchaseList", "");

	boost::property_tree::ptree element_tree;
	element_tree.put("id", 1);
	element_tree.put("name", "Базовый словарь + анализатор слов");
	element_tree.put("price", 200);
	element_tree.put("version", 1);
	p.add_child("OnGetPurchaseList.Purchases.Purchase", element_tree);

	SendPropertyTree(p);
}

void TUser::Send_OnPurchase(int purchaseId, std::string username, std::string purchaseCode)
{


	std::string codehash = CalcPurchaseCheckCode(purchaseId, username);


	if (codehash == purchaseCode)
	{

		HallyuSocketServer.AddPurchaseForUser(username, purchaseId);

		boost::property_tree::ptree p;

		p.put("OnPurchase", "Succeed");

		SendPropertyTree(p);
	}
	else
	{
		boost::property_tree::ptree p;

		p.put("OnPurchase", "Failed");

		SendPropertyTree(p);
	}
}

void TUser::Send_OnDownloadPurchase(int purchaseId, std::string username, std::string purchaseCode)
{

	SE::WriteToLog("Send_OnDownloadPurchase begin");

	std::string codehash = CalcPurchaseCheckCode(purchaseId, username);

	
	SE::WriteToLog("Send_OnDownloadPurchase codehash calculated: " + codehash);

	if (codehash == purchaseCode)
	{

		SE::WriteToLog("Send_OnDownloadPurchase codehash == purchaseCode");

		std::vector<int> purchaseList = HallyuSocketServer.GetPurchaseList(username);

		SE::WriteToLog("Send_OnDownloadPurchase purchaseList size = " + boost::lexical_cast<std::string>(purchaseList.size()));

		bool purchaseMade = false;
		for (int purchase : purchaseList)
		{
			if (purchase == purchaseId)
			{
				purchaseMade = true;
				break;
			}
		}

		SE::WriteToLog("Send_OnDownloadPurchase purchaseMade calculated");
#if 0
		if (purchaseMade)
		{

			SE::WriteToLog("Send_OnDownloadPurchase purchaseMade = true");

			boost::mutex::scoped_lock lock(HallyuSocketServer.zipPurchaseMutex);

			SE::WriteToLog("Send_OnDownloadPurchase passed mutex");

#ifdef _WIN32
			const char* zipFilename = "C:\\Workplace\\Projects\\hallyu\\hallyu_server\\scripts\\purchase.zip";
#else
			const char* zipFilename = "/home/devuser/workplace/hallyu_server/linux/scripts/purchase.zip";
#endif
			SE::WriteToLog("Send_OnDownloadPurchase before std::remove(zipFilename)");

			int r = std::remove(zipFilename);
			if (r < 0)
			{
				SE::WriteToLog("Error: Failed to delete archive purchase.zip");
			}

			SE::WriteToLog("Send_OnDownloadPurchase after std::remove(zipFilename)");
#ifdef _WIN32

			ZipFile::AddEncryptedFile(zipFilename, "C:\\Workplace\\Projects\\hallyu\\hallyu_server\\scripts\\main.lua", "main.lua", "mainpass");
			ZipFile::AddEncryptedFile(zipFilename, "C:\\Workplace\\Projects\\hallyu\\hallyu_server\\scripts\\1000_noun.txt", "1000_noun.txt", "mainpass");
			ZipFile::AddEncryptedFile(zipFilename, "C:\\Workplace\\Projects\\hallyu\\hallyu_server\\scripts\\1000_verb.txt", "1000_verb.txt", "mainpass");
			ZipFile::AddEncryptedFile(zipFilename, "C:\\Workplace\\Projects\\hallyu\\hallyu_server\\scripts\\1000_special.txt", "1000_special.txt", "mainpass");
#else
			ZipFile::AddEncryptedFile(zipFilename, "/home/devuser/workplace/hallyu_server/linux/scripts/main.lua", "main.lua", "mainpass");
			SE::WriteToLog("Send_OnDownloadPurchase AddEncryptedFile main.lua");
			ZipFile::AddEncryptedFile(zipFilename, "/home/devuser/workplace/hallyu_server/linux/scripts/1000_noun.txt", "1000_noun.txt", "mainpass");
			SE::WriteToLog("Send_OnDownloadPurchase AddEncryptedFile 1000_noun.txt");
			ZipFile::AddEncryptedFile(zipFilename, "/home/devuser/workplace/hallyu_server/linux/scripts/1000_verb.txt", "1000_verb.txt", "mainpass");
			SE::WriteToLog("Send_OnDownloadPurchase AddEncryptedFile 1000_verb.txt");
			ZipFile::AddEncryptedFile(zipFilename, "/home/devuser/workplace/hallyu_server/linux/scripts/1000_special.txt", "1000_special.txt", "mainpass");
			SE::WriteToLog("Send_OnDownloadPurchase AddEncryptedFile 1000_special.txt");
#endif
			std::ifstream zipFile(zipFilename, std::ios::binary);

			SE::WriteToLog("Send_OnDownloadPurchase AddEncryptedFile zipFile");

			std::vector<char> zipFileContent((std::istreambuf_iterator<char>(zipFile)),
				std::istreambuf_iterator<char>());

			SE::WriteToLog("Send_OnDownloadPurchase send binary, size = " + boost::lexical_cast<std::string>(zipFileContent.size()));

			SendBinary(zipFileContent);
		}
		else
		{
			SE::WriteToLog("Send_OnDownloadPurchase send binary zero size because purchaseMade = false");

			SendBinary(std::vector<char>());
		}
		
#endif
		SendBinary(std::vector<char>());
	}
	else
	{
		SE::WriteToLog("Send_OnDownloadPurchase send binary zero size because codehash wrong");
		SendBinary(std::vector<char>());
	}

	SE::WriteToLog("Send_OnDownloadPurchase end");
}


#endif

//===================== THallyuSocketServer =================

void THallyuSocketServer::StartAccept()
{


	acceptor.async_accept(
		[this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
	{
		try
		{
			if (!ec)
			{

				std::shared_ptr<TUser> user = std::shared_ptr<TUser>(new TUser(IoService, std::move(socket), *this));

				user->BeforeStartRead();

				user->StartRead();
			}


			StartAccept();
		}
		catch (std::exception e)
		{
			SE::WriteToLog("Error in inner TServerSocket::StartAccept");
		}
	});
}


THallyuSocketServer::THallyuSocketServer(int port, TMySqlConnector& mySqlConnector, LH::LuaHelper& iLuaHelper)
	: SE::TServerSocket(port)
	, MySqlConnector(mySqlConnector)
	, ImageDownloader(IoService)
	, luaHelper(iLuaHelper)
	, io_context_(1) // ===== http -----
	, signals_(io_context_) // ===== http -----
	, acceptor_(io_context_) // ====== http -----
{
	// -------# http #-------
	std::string addr_; // replace it
	boost::asio::ip::tcp::resolver resolver_(io_context_);
	boost::asio::ip::tcp::endpoint endpoint_= *resolver_.resolve(addr_,std::to_string(port)).begin();
	// ----------------------
	acceptor_.open(endpoint_.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint_);
	acceptor_.listen();
	// =======# http #=======


	SE::WriteToLog("THallyuSocketServer::THallyuSocketServer begin");

	StartAccept();

	ServiceThread = boost::thread(std::bind(&TServerSocket::UpdateInThread, this));

	SE::WriteToLog("THallyuSocketServer::THallyuSocketServer end");

}


#ifdef USE_MYSQL
std::vector<TPostListElement> THallyuSocketServer::GetPostList(int feedId, int startingFromPostId, int count)
{
	std::vector<TPostListElement> postList = MySqlConnector.GetPostList(feedId, startingFromPostId, count);

	for (auto& post : postList)
	{

		TUserListElement user = MySqlConnector.GetUserByUserLogin(post.Username);

		post.Username = user.firstName + " " + user.lastName;

	}

	return postList;
}

TPostListElement THallyuSocketServer::GetPostListElement(int id)
{
	return MySqlConnector.GetPostListElement(id);
}


std::vector<TFeedListElement> THallyuSocketServer::GetFeedListElementArr()
{
	return MySqlConnector.GetFeedListElementArr();
}

TFeedListElement THallyuSocketServer::GetFeedListElement(int id)
{
	return MySqlConnector.GetFeedListElement(id);
}


void THallyuSocketServer::RegisterVkUser(const std::string& vkLogin, const std::string& firstName, const std::string& lastName, const std::string& photo200Url)
{

	std::string photoFileName = "userphotos/photo" + vkLogin + ".jpg";

	ImageDownloader.DownloadImageToFile(photo200Url, photoFileName);

	MD5 md5;

	std::string photo200Hash = md5.digestFile((TImageDownloader::getFilePath() + photoFileName).c_str());


	MySqlConnector.RegisterVkUser(vkLogin, firstName, lastName, photoFileName, photo200Hash);
}

TUserListElement THallyuSocketServer::GetUserByVkLogin(const std::string& vkLogin)
{
	return MySqlConnector.GetUserByVkLogin(vkLogin);
}

TUserListElement THallyuSocketServer::GetUserByUserLogin(const std::string& username)
{
	return MySqlConnector.GetUserByUserLogin(username);
}

void THallyuSocketServer::AddComment(const std::string& username, int postId, const std::string& commentText)
{
	std::string clearCommentText = boost::trim_copy(commentText);
	if (clearCommentText.size() > 0)
	{
		MySqlConnector.AddComment(username, postId, clearCommentText);

	}
}

std::vector<TCommentStruct> THallyuSocketServer::GetComments(int postId)
{
	std::vector<TCommentListElement> commentListElements = MySqlConnector.GetComments(postId);

	std::vector<TCommentStruct> result;

	for (auto& comment : commentListElements)
	{
		TUserListElement user = MySqlConnector.GetUserByUserLogin(comment.UserName);

		TCommentStruct commentStruct;

		commentStruct.CommentId = comment.CommentId;
		commentStruct.CommentText = comment.CommentText;
		commentStruct.ImageUrl = comment.ImageUrl;
		commentStruct.ImageHash = comment.ImageHash;
		commentStruct.AuthorName = user.firstName + " " + user.lastName;
		commentStruct.AuthorPhotoUrl = user.photoUrl;
		commentStruct.AuthorPhotoHash = user.photoHash;
		commentStruct.Date = comment.Date;

		result.push_back(commentStruct);
	}

	return result;
}

void THallyuSocketServer::AddPost(const std::string& username, int feedId, const std::string& postHeader, const std::string& postText)
{
	std::string clearPostHeader = boost::trim_copy(postHeader);
	std::string clearPostText = boost::trim_copy(postText);
	if (clearPostHeader.size() > 0 && clearPostText.size() > 0)
	{
		MySqlConnector.AddPost(username, feedId, clearPostHeader, clearPostText);
	}
}

void THallyuSocketServer::AddPostWithImage(const std::string& username, int feedId, const std::string& postHeader, const std::string& postText, const std::string& imageUrl, const std::string& imageHash)
{
	std::string clearPostHeader = boost::trim_copy(postHeader);
	std::string clearPostText = boost::trim_copy(postText);
	if (clearPostHeader.size() > 0 && clearPostText.size() > 0)
	{
		MySqlConnector.AddPostWithImage(username, feedId, clearPostHeader, clearPostText, imageUrl, imageHash);
	}
}


std::vector<TComicsSeriesListElement> THallyuSocketServer::GetComicsSeries()
{
	return MySqlConnector.GetComicsSeries();
}

std::vector<TComicsEpisodeListElement> THallyuSocketServer::GetComicsEpisodes(int seriesId)
{
	return MySqlConnector.GetComicsEpisodes(seriesId);
}

std::vector<TComicsPictureListElement> THallyuSocketServer::GetComicsPictures(int episodeId)
{
	return MySqlConnector.GetComicsPictures(episodeId);
}

std::vector<int> THallyuSocketServer::GetPurchaseList(const std::string& username)
{
	return MySqlConnector.GetPurchaseListForUser(username);
}

std::vector<std::pair<int, int>> THallyuSocketServer::GetPurchaseListWithVersion(const std::string& username)
{

	std::vector<int> purchaseIdArr = MySqlConnector.GetPurchaseListForUser(username);

	std::vector<std::pair<int, int>> result;

	for (int i = 0; i < purchaseIdArr.size(); i++)
	{
		if (purchaseIdArr[i] == 1)
		{
			result.push_back(std::make_pair(1, CONST_DICTIONARY_CURRENT_VERSION)); //id and version
		}
	}

	return result;
}

void THallyuSocketServer::AddPurchaseForUser(const std::string& username, int purchaseId)
{
	MySqlConnector.AddPurchaseForUser(username, purchaseId);
}

#endif
