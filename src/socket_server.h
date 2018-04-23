#ifndef SOCKET_SERVER_H_INCLUDED
#define SOCKET_SERVER_H_INCLUDED

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/array.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/signals2.hpp"
#include "boost/thread.hpp"

#include "MySqlConnector.h"

#include "HangulProcessor.h"
#include "LuaHelper.h"
#include "ImageDownloader.h"

#include "SE/Server.h"
#include "SE/Network.h"


extern const int CONST_DICTIONARY_CURRENT_VERSION;



class THallyuSocketServer;


std::string CalcVkCheckCode(const std::string& vkLogin);



//Xperimental -- maybe we should use Inner Join for such stuff?
struct TCommentStruct
{
	int CommentId;
	std::string CommentText;
	std::string ImageUrl;
	std::string ImageHash;
	std::string AuthorName;
	std::string AuthorPhotoUrl;
	std::string AuthorPhotoHash;
	std::string Date;
};

class TUser : public std::enable_shared_from_this<TUser>
{
public:

	static int counter;
	int selfCounter;

	TUser(boost::asio::io_service& ioService, boost::asio::ip::tcp::socket socket, THallyuSocketServer& server);

	~TUser();


	
	void OnReceive_RegisterViaVk(boost::property_tree::ptree propertyTree);

	void OnReceive_RequestWordTranslation(boost::property_tree::ptree propertyTree);
	void OnReceive_RequestCard(boost::property_tree::ptree propertyTree);
	void OnReceive_RequestChineseNumberRecognize(boost::property_tree::ptree propertyTree);

#ifdef USE_MYSQL


	void OnReceive_GetFeed(boost::property_tree::ptree propertyTree);
	void OnReceive_GetFeedList(boost::property_tree::ptree propertyTree);

	void OnReceive_GetUser(boost::property_tree::ptree propertyTree);
	void OnReceive_AddComment(boost::property_tree::ptree propertyTree);
	void OnReceive_GetComments(boost::property_tree::ptree propertyTree);

	void OnReceive_AddPost(boost::property_tree::ptree propertyTree);

	void OnReceive_GetComicsSeries(boost::property_tree::ptree propertyTree);
	void OnReceive_GetComicsEpisodes(boost::property_tree::ptree propertyTree);
	void OnReceive_GetComicsPictures(boost::property_tree::ptree propertyTree);

	void OnReceive_GetPurchaseList(boost::property_tree::ptree propertyTree);
	void OnReceive_Purchase(boost::property_tree::ptree propertyTree);
	void OnReceive_DownloadPurchase(boost::property_tree::ptree propertyTree);


	void ProcessBinaryData(boost::property_tree::ptree propertyTree, std::vector<char> binary);

#endif

	void Send_OnRequestWordTranslation(std::string wordToTranslate);
	void Send_OnRequestCard(size_t wordCount);
	void Send_OnRequestChineseNumberRecognize(int maxDigits);

#ifdef USE_MYSQL

	void Send_OnGetFeed(int feedId, int startingFromPostId, int count);
	void Send_OnGetFeedList();

	void Send_OnRegisterViaVk(const std::string& username);
	void OnReceive_RegisterViaVk_Api2(boost::property_tree::ptree propertyTree);

	void Send_OnGetUser(const std::string& username);
	void Send_OnAddComment();
	void Send_OnGetComments(int postId);

	void Send_OnAddPost();

	void Send_OnGetComicsSeries();
	void Send_OnGetComicsEpisodes(int seriesId);
	void Send_OnGetComicsPictures(int episodeId);

	void Send_OnGetPurchaseList();
	void Send_OnPurchase(int purchaseId, std::string username, std::string purchaseCode);
	void Send_OnDownloadPurchase(int purchaseId, std::string username, std::string purchaseCode);


#endif

	

	std::string Address;

	THallyuSocketServer& HallyuSocketServer;


	boost::asio::io_service& IoService;

	boost::asio::ip::tcp::socket Socket;


	int DataSize;

#ifdef USE_MYSQL

	void InnerStartReadBinary(boost::property_tree::ptree binaryTree);

#endif

	std::vector<char> Data;

	SE::TDataReadSignalMap DataReadSignalMap;

	void BeforeStartRead();
	void StartRead();

	void HandleReadDataSize(const boost::system::error_code& error);

	void HandleReadData(const boost::system::error_code& error);

#ifdef USE_MYSQL

	void HandleReadBinaryData(boost::property_tree::ptree binaryTree, const boost::system::error_code& error);
#endif

	void SendPropertyTree(boost::property_tree::ptree pTree);

	void SendBinary(const std::vector<char>& binaryData);
};

class THallyuSocketServer : public SE::TServerSocket
{
public:
	//THangulProcessor& HangulProcessor;
	LH::LuaHelper& luaHelper;

	void StartAccept();

	THallyuSocketServer(int port, TMySqlConnector& mySqlConnector, LH::LuaHelper& iLuaHelper);

	// ---# http #---
	boost::asio::io_context io_context_;
	boost::asio::signal_set signals_;
	boost::asio::ip::tcp::acceptor acceptor_;
	// ===# http #===

#ifdef USE_MYSQL

	std::vector<TPostListElement> GetPostList(int feedId, int startingFromPostId, int count);
	TPostListElement GetPostListElement(int id);

	std::vector<TFeedListElement> GetFeedListElementArr();
	TFeedListElement GetFeedListElement(int id);

	void RegisterVkUser(const std::string& vkLogin, const std::string& firstName, const std::string& lastName, const std::string& photo200Url);
	TUserListElement GetUserByVkLogin(const std::string& vkLogin);
	TUserListElement GetUserByUserLogin(const std::string& username);

	void AddComment(const std::string& username, int postId, const std::string& commentText);
	std::vector<TCommentStruct> GetComments(int postId);

	void AddPost(const std::string& username, int feedId, const std::string& postHeader, const std::string& postText);
	void AddPostWithImage(const std::string& username, int feedId, const std::string& postHeader, const std::string& postText, const std::string& imageUrl, const std::string& imageHash);

	std::vector<TComicsSeriesListElement> GetComicsSeries();

	std::vector<TComicsEpisodeListElement> GetComicsEpisodes(int seriesId);

	std::vector<TComicsPictureListElement> GetComicsPictures(int episodeId);

	std::vector<int> GetPurchaseList(const std::string& username);

	std::vector<std::pair<int, int>> GetPurchaseListWithVersion(const std::string& username);

	void AddPurchaseForUser(const std::string& username, int purchaseId);

#endif

	boost::mutex zipPurchaseMutex;


protected:

	TMySqlConnector& MySqlConnector;

	TImageDownloader ImageDownloader;

	std::vector<std::shared_ptr<TUser>> UserArr;

	boost::mutex logMutex;

	
	
};

#endif //SOCKET_SERVER_H_INCLUDED