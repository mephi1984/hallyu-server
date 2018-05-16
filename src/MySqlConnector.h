#ifndef MYSQL_CONNECTOR_H_INCLUDED
#define MYSQL_CONNECTOR_H_INCLUDED

#include <vector>
#include <memory>

#include "boost/lexical_cast.hpp"
#include "boost/date_time.hpp"
#include "boost/thread/mutex.hpp"

#include "SE/misc.h"

#ifdef USE_MYSQL

#ifdef _WIN32

#include "driver/mysql_driver.h"

#include "driver/mysql_public_iface.h"
#else

#include "mysql_driver.h"

#include "mysql_public_iface.h"

#endif



struct TPostListElement
{
	int Id;
	std::string Header;
	std::string Text;
	std::string ImageUrl;
	std::string ImageHash;
	std::string Username;
	std::string Date;
};

struct TFeedListElement
{
	int Id;
	std::string Name;
	int CommentMode;
	int PostMode;

	TFeedListElement()
		: Id(1)
		, Name("")
		, CommentMode(0)
		, PostMode(0)
	{
	}
};


struct TUserListElement
{
	std::string login;
	int postCount;
	std::string firstName;
	std::string lastName;
	std::string photoUrl;
	std::string photoHash;

	TUserListElement()
		: postCount(0)
	{
	}
};

struct TCommentListElement
{
	int CommentId;
	std::string CommentText;
	std::string ImageUrl;
	std::string ImageHash;
	std::string UserName;
	std::string Date;
};


struct TComicsSeriesListElement
{
	int seriesId;
	std::string translatedTitle;
	std::string originalTitle;
	std::string description;
	int episodesCount;
	std::string translatedImageUrl;
	std::string translatedImageHash;
	std::string originalImageUrl;
	std::string originalImageHash;
};

struct TComicsEpisodeListElement
{
	int episodeId;
	int seriesId;
	std::string title;
	std::string description;
	int position;
	int picturesCount;
};


struct TComicsPictureListElement
{
	int pictureId;
	int episodeId;
	int position;
	std::string translatedImageUrl;
	std::string translatedImageHash;
	std::string originalImageUrl;
	std::string originalImageHash;
};



class TMySqlConnector
{

protected:
	bool Connect();

	void ReconnectIfNeeded();

	void PrepareStatements();

	std::shared_ptr<sql::Connection> Connection;

	boost::mutex preparedStatementMutex;

	std::shared_ptr<sql::PreparedStatement> psGetPostList;
	std::shared_ptr<sql::PreparedStatement> psGetPostListStartingFrom;

	std::shared_ptr<sql::PreparedStatement> psGetPostListElement;
	std::shared_ptr<sql::PreparedStatement> psGetFeedListElementArr;
	std::shared_ptr<sql::PreparedStatement> psGetFeedListElement;
	std::shared_ptr<sql::PreparedStatement> psGetCountVkUserWithLogin;

	std::shared_ptr<sql::PreparedStatement> psInsertVkUserWithoutPhotoHash;
	std::shared_ptr<sql::PreparedStatement> psInsertVkUserWithPhotoHash;

	std::shared_ptr<sql::PreparedStatement> psUpdateVkUserWithoutPhotoHash;
	std::shared_ptr<sql::PreparedStatement> psUpdateVkUserWithPhotoHash;

	std::shared_ptr<sql::PreparedStatement> psGetUserByVkLogin;
	std::shared_ptr<sql::PreparedStatement> psGetUserByUserLogin;

	

	std::shared_ptr<sql::PreparedStatement> psAddComment;
	std::shared_ptr<sql::PreparedStatement> psGetComments20;

	std::shared_ptr<sql::PreparedStatement> psAddPost;
	std::shared_ptr<sql::PreparedStatement> psAddPostWithImage;
	std::shared_ptr<sql::PreparedStatement> psAddPostWithImageWithHash;

	std::shared_ptr<sql::PreparedStatement> psIncreasePostCounterForUser;

	std::shared_ptr<sql::PreparedStatement> psGetComicsSeries;
	std::shared_ptr<sql::PreparedStatement> psGetComicsEpisodes;
	std::shared_ptr<sql::PreparedStatement> psGetComicsPictures;

	std::shared_ptr<sql::PreparedStatement> psAddPurchaseForUser;
	std::shared_ptr<sql::PreparedStatement> psGetPurchaseListByUser;

public:

	TMySqlConnector();

	std::vector<TPostListElement> GetPostList(int feedId, int startingFromPostId, int count);
	TPostListElement GetPostListElement(int id);

	
	std::vector<TFeedListElement> GetFeedListElementArr();
	TFeedListElement GetFeedListElement(int id);
	


	void RegisterVkUser(const std::string& vkLogin, const std::string& firstName, const std::string& lastName, const std::string& photo200Url, const std::string& photo200Hash);
	TUserListElement GetUserByVkLogin(const std::string& vkLogin);
	TUserListElement GetUserByUserLogin(const std::string& userLogin);

	void AddComment(const std::string& username, int postId, std::string commentText);

	std::vector<TCommentListElement> GetComments(int postId);

	void AddPost(const std::string& username, int feedId, std::string postHeader, std::string postText);
	void AddPostWithImage(const std::string& username, int feedId, std::string postHeader, std::string postText, const std::string& imageUrl, const std::string& imageHash);

	void IncreasePostCounterForUser(const std::string& username);

	std::vector<TComicsSeriesListElement> GetComicsSeries();

	std::vector<TComicsEpisodeListElement> GetComicsEpisodes(int seriesId);

	std::vector<TComicsPictureListElement> GetComicsPictures(int episodeId);

	std::vector<int> GetPurchaseListForUser(const std::string& username);
	void AddPurchaseForUser(const std::string& username, int purchaseId);

protected:
	std::string GenerateLogin(const std::string& userId);

};

#else

class TMySqlConnector //Dummy
{
};

#endif



#endif
