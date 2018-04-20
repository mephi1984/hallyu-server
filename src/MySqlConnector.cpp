#include "MySqlConnector.h"


#ifdef USE_MYSQL

TMySqlConnector::TMySqlConnector()
{
	Connect();
	PrepareStatements();
}

bool TMySqlConnector::Connect()
{
	SE::WriteToLog("TMySqlConnector::Connect begin");

	sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();

	if (driver == nullptr)
	{
		SE::WriteToLog("Error: MySQL Driver not found");
		return false;
	}

	Connection = std::shared_ptr<sql::Connection>(driver->connect("tcp://mysql-oregon-alpha:3306", "hallyu", "OrOGgH3NOrOGgH3NOrOGgH3N"));
	//Connection = std::shared_ptr<sql::Connection>(driver->connect("tcp://localhost:3306", "hallyu", "OrOGgH3N"));

	if (Connection == nullptr)
	{
		SE::WriteToLog("Error: Connection is nullptr");
		return false;
	}

	std::shared_ptr<sql::Statement> stmt(Connection->createStatement());

	stmt->execute("USE hallyu");

	
	SE::WriteToLog("TMySqlConnector::Connect end");

	return true;

}

void TMySqlConnector::ReconnectIfNeeded()
{
	/*
	if (Connection == nullptr || Connection->isClosed())
	{
		Connect();
		PrepareStatements();
	}*/

	bool needReconnect = false;

	if (Connection == nullptr || Connection->isClosed())
	{
		SE::WriteToLog("TMySqlConnector::ReconnectIfNeeded: (Connection == nullptr || Connection->isClosed()) == true");
		needReconnect = true;
	}
	else{

		SE::WriteToLog("TMySqlConnector::ReconnectIfNeeded: (Connection == nullptr || Connection->isClosed()) == false");

		try {
			std::shared_ptr<sql::Statement> stmt(Connection->createStatement());

			std::shared_ptr< sql::ResultSet >res = std::shared_ptr< sql::ResultSet >(stmt->executeQuery("SELECT 1"));
		}
		catch (sql::SQLException e)
		{
			needReconnect = true;
			SE::WriteToLog("Error in TMySqlConnector::ReconnectIfNeeded: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
		}
		catch (std::exception e)
		{
			needReconnect = true;
			SE::WriteToLog("std::exception happened in TMySqlConnector::ReconnectIfNeeded " + std::string(e.what()));
		}
	}


	if (needReconnect)
	{
		SE::WriteToLog("TMySqlConnector::ReconnectIfNeeded needReconnect = true");
		Connect();
		PrepareStatements();
	}
}


void TMySqlConnector::PrepareStatements()
{
	try
	{
		SE::WriteToLog("TMySqlConnector::PrepareStatements begin");

		boost::mutex::scoped_lock lock(preparedStatementMutex);

		psGetPostList = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, header, text, imageUrl, imageHash, username, date FROM t_posts WHERE (feedId = ?) ORDER BY id DESC LIMIT ?"));
		psGetPostListStartingFrom = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, header, text, imageUrl, imageHash, username, date FROM t_posts WHERE (feedId = ?) AND (id < ?) ORDER BY id DESC LIMIT ?"));

		psGetPostListElement = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, header, text, imageUrl, username, date FROM t_posts WHERE id = ?"));

		psGetFeedListElementArr = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, name, commentMode, postMode FROM t_feeds"));

		psGetFeedListElement = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, name, commentMode, postMode FROM t_feeds WHERE id = ?"));

		psGetCountVkUserWithLogin = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT COUNT(*) FROM tbl_web_user WHERE vkLogin = ?"));


		psInsertVkUserWithoutPhotoHash = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("INSERT INTO tbl_web_user (username, password, firstName, lastName, vkLogin, photoUrl)  VALUES(?, ?, ?, ?, ?, ?)"));
		psInsertVkUserWithPhotoHash = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("INSERT INTO tbl_web_user (username, password, firstName, lastName, vkLogin, photoUrl, photoHash)  VALUES(?, ?, ?, ?, ?, ?, ?)"));

		psUpdateVkUserWithoutPhotoHash = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("UPDATE tbl_web_user SET password=?, firstName=?, lastName=?, vkLogin=?, photoUrl=? WHERE username=?"));
		psUpdateVkUserWithPhotoHash = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("UPDATE tbl_web_user SET password=?, firstName=?, lastName=?, vkLogin=?, photoUrl=?, photoHash=? WHERE username=?"));

		psGetUserByVkLogin = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT username, postCount, firstName, lastName, photoUrl, photoHash FROM tbl_web_user WHERE vkLogin = ?"));


		psGetUserByUserLogin = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT username, postCount, firstName, lastName, photoUrl, photoHash FROM tbl_web_user WHERE username = ?"));

		psAddComment = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("INSERT INTO t_comments(postId, text, username, date)  VALUES(?, ?, ?, ?)"));


		psGetComments20 = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, text, imageUrl, imageHash, username, date FROM t_comments WHERE postId = ? ORDER BY id DESC LIMIT 20"));

		psAddPost = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("INSERT INTO t_posts (feedId, header, text, username, date)  VALUES(?, ?, ?, ?, ?)"));
		psAddPostWithImage = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("INSERT INTO t_posts (feedId, header, text, username, date, imageUrl)  VALUES(?, ?, ?, ?, ?, ?)"));
		psAddPostWithImageWithHash = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("INSERT INTO t_posts(feedId, header, text, username, date, imageUrl, imageHash)  VALUES(?, ?, ?, ?, ?, ?, ?)"));

		psIncreasePostCounterForUser = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("UPDATE tbl_web_user SET postCount = (postCount + 1) WHERE username = ?"));

		psGetComicsSeries = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, translatedTitle, originalTitle, description, episodesCount, translatedImageUrl, translatedImageHash, originalImageUrl, originalImageHash FROM t_comics_series ORDER BY id"));
		psGetComicsEpisodes = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, seriesId, title, description, position, picturesCount FROM t_comics_episodes WHERE seriesId = ? ORDER BY position"));
		psGetComicsPictures = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT id, episodeId, position, translatedImageUrl, translatedImageHash, originalImageUrl, originalImageHash FROM t_comics_pictures WHERE episodeId = ? ORDER BY position"));

		psAddPurchaseForUser = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("INSERT INTO t_user_purchases (username, purchaseId, dateTime)  VALUES(?, ?, ?)"));
		psGetPurchaseListByUser = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT purchaseId FROM t_user_purchases WHERE username=?"));

		SE::WriteToLog("TMySqlConnector::PrepareStatements end");
	}
	catch (const std::exception& e)
	{
		SE::WriteToLog(std::string("Exception in TMySqlConnector::PrepareStatements ") + e.what());
	}
}


std::vector<TPostListElement> TMySqlConnector::GetPostList(int feedId, int startingFromPostId, int count)
{

	std::vector<TPostListElement> result;

	SE::WriteToLog("TMySqlConnector::GetPostList begin");

	ReconnectIfNeeded();

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);
		
		std::shared_ptr< sql::ResultSet > res;

		if (startingFromPostId == -1)
		{

			psGetPostList->setInt(1, feedId);
			psGetPostList->setInt(2, count);

			if (psGetPostList->execute())
			{
				res = std::shared_ptr< sql::ResultSet >(psGetPostList->getResultSet());
			}

		}
		else
		{
			psGetPostListStartingFrom->setInt(1, feedId);
			psGetPostListStartingFrom->setInt(2, count);
			psGetPostListStartingFrom->setInt(3, startingFromPostId);

			if (psGetPostListStartingFrom->execute())
			{
				res = std::shared_ptr< sql::ResultSet >(psGetPostListStartingFrom->getResultSet());
			}
		}

		if (res != 0)
		{

			while (res->next())
			{
				TPostListElement r;
				r.Id = res->getInt(1);
				r.Header = res->getString(2);
				r.Text = res->getString(3);

				std::string imageUrl = res->getString(4);

				if (imageUrl != "")
				{
					imageUrl = "hallyu.ru/" + imageUrl;
				}
				r.ImageUrl = imageUrl;

				r.ImageHash = res->getString(5);


				r.Username = res->getString(6);
				r.Date = res->getString(7);
				result.push_back(r);
			}

		}

	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetPostList: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetPostList " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetPostList result size = " + boost::lexical_cast<std::string>(result.size()));

	SE::WriteToLog("TMySqlConnector::GetPostList end");

	return result;
}

TPostListElement TMySqlConnector::GetPostListElement(int id)
{
	SE::WriteToLog("TMySqlConnector::GetPostListElement begin");

	ReconnectIfNeeded();

	TPostListElement result;
	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);
		
		psGetPostListElement->setInt(1, id);
		
		if (psGetPostListElement->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetPostListElement->getResultSet());


			if (res->next())
			{

				result.Id = res->getInt(1);
				result.Header = res->getString(2);
				result.Text = res->getString(3);

				std::string imageUrl = res->getString(4);

				if (imageUrl != "")
				{
					imageUrl = "hallyu.ru/" + imageUrl;
				}
				result.ImageUrl = imageUrl;

				result.Username = res->getString(5);
				result.Date = res->getString(6);

				SE::WriteToLog("TMySqlConnector::GetPostListElement psGetPostListElement succeed");
			}
		}


	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetPostListElement: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetPostListElement " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetPostListElement end");


	return result;
}




std::vector<TFeedListElement> TMySqlConnector::GetFeedListElementArr()
{
	std::vector<TFeedListElement> result;

	SE::WriteToLog("TMySqlConnector::GetFeedListElementArr begin");

	ReconnectIfNeeded();

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);

		if (psGetFeedListElementArr->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetFeedListElementArr->getResultSet());

			while (res->next())
			{
				TFeedListElement element;
				element.Id = res->getInt(1);
				element.Name = res->getString(2);
				element.CommentMode = res->getInt(3);
				element.PostMode = res->getInt(4);
				result.push_back(element);
			}

		}

	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetFeedListElementArr: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetFeedListElementArr " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetFeedListElementArr result size = " + boost::lexical_cast<std::string>(result.size()));

	SE::WriteToLog("TMySqlConnector::GetFeedListElementArr end");

	return result;
}

TFeedListElement TMySqlConnector::GetFeedListElement(int id)
{

	TFeedListElement result;

	SE::WriteToLog("TMySqlConnector::GetFeedListElement begin");

	ReconnectIfNeeded();

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);

		psGetFeedListElement->setInt(1, id);

		if (psGetFeedListElement->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetFeedListElement->getResultSet());


			if (res->next())
			{
				result.Id = res->getInt(1);
				result.Name = res->getString(2);
				result.CommentMode = res->getInt(3);
				result.PostMode = res->getInt(4);

				SE::WriteToLog("TMySqlConnector::GetFeedListElement psGetFeedListElement succeed");
			}
		}
	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetFeedListElement: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetFeedListElement " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetFeedListElement end");


	return result;
}


void TMySqlConnector::RegisterVkUser(const std::string& vkLogin, const std::string& firstName, const std::string& lastName, const std::string& photo200Url, const std::string& photo200Hash)
{

	SE::WriteToLog("TMySqlConnector::RegisterVkUser begin");

	ReconnectIfNeeded();


	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);
		
		psGetCountVkUserWithLogin->setString(1, vkLogin);

		if (psGetCountVkUserWithLogin->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetCountVkUserWithLogin->getResultSet());

			if (res->next())
			{

				if (res->getInt(1) == 0)
				{
					std::string login = GenerateLogin(vkLogin);
					std::string password = "";

					if (photo200Hash == "")
					{
						psInsertVkUserWithoutPhotoHash->setString(1, login);
						psInsertVkUserWithoutPhotoHash->setString(2, password);
						psInsertVkUserWithoutPhotoHash->setString(3, firstName);
						psInsertVkUserWithoutPhotoHash->setString(4, lastName);
						psInsertVkUserWithoutPhotoHash->setString(5, vkLogin);
						psInsertVkUserWithoutPhotoHash->setString(6, photo200Url);

						if (psInsertVkUserWithoutPhotoHash->execute())
						{
							SE::WriteToLog("TMySqlConnector::RegisterVkUser psInsertVkUserWithoutPhotoHash executed successfully");
						}
					}
					else
					{
						psInsertVkUserWithPhotoHash->setString(1, login);
						psInsertVkUserWithPhotoHash->setString(2, password);
						psInsertVkUserWithPhotoHash->setString(3, firstName);
						psInsertVkUserWithPhotoHash->setString(4, lastName);
						psInsertVkUserWithPhotoHash->setString(5, vkLogin);
						psInsertVkUserWithPhotoHash->setString(6, photo200Url);
						psInsertVkUserWithPhotoHash->setString(7, photo200Hash);

						if (psInsertVkUserWithPhotoHash->execute())
						{
							SE::WriteToLog("TMySqlConnector::RegisterVkUser psInsertVkUserWithPhotoHash executed successfully");
						}
					}

				}
				else
				{
					std::string login = GenerateLogin(vkLogin);
					std::string password = "";

					if (photo200Hash == "")
					{
						
						psUpdateVkUserWithoutPhotoHash->setString(1, password);
						psUpdateVkUserWithoutPhotoHash->setString(2, firstName);
						psUpdateVkUserWithoutPhotoHash->setString(3, lastName);
						psUpdateVkUserWithoutPhotoHash->setString(4, vkLogin);
						psUpdateVkUserWithoutPhotoHash->setString(5, photo200Url);

						psUpdateVkUserWithoutPhotoHash->setString(6, login);

						if (psUpdateVkUserWithoutPhotoHash->execute())
						{
							SE::WriteToLog("TMySqlConnector::RegisterVkUser psUpdateVkUserWithoutPhotoHash executed successfully");
						}
					}
					else
					{
						psUpdateVkUserWithPhotoHash->setString(1, password);
						psUpdateVkUserWithPhotoHash->setString(2, firstName);
						psUpdateVkUserWithPhotoHash->setString(3, lastName);
						psUpdateVkUserWithPhotoHash->setString(4, vkLogin);
						psUpdateVkUserWithPhotoHash->setString(5, photo200Url);
						psUpdateVkUserWithPhotoHash->setString(6, photo200Hash);

						psUpdateVkUserWithPhotoHash->setString(7, login);

						if (psUpdateVkUserWithPhotoHash->execute())
						{
							SE::WriteToLog("TMySqlConnector::RegisterVkUser psUpdateVkUserWithPhotoHash executed successfully");
						}
					}
				}
			}

		}

	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::RegisterVkUser: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::RegisterVkUser " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::RegisterVkUser end");


}

TUserListElement TMySqlConnector::GetUserByVkLogin(const std::string& vkLogin)
{
	TUserListElement result;

	SE::WriteToLog("TMySqlConnector::GetUserByVkLogin begin");

	ReconnectIfNeeded();

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);

		psGetUserByVkLogin->setString(1, vkLogin);

		if (psGetUserByVkLogin->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetUserByVkLogin->getResultSet());


			if (res->next())
			{
				result.login = res->getString(1);
				result.postCount = res->getInt(2);
				result.firstName = res->getString(3);
				result.lastName = res->getString(4);
				result.photoUrl = res->getString(5);
				result.photoHash = res->getString(6);

				SE::WriteToLog("TMySqlConnector::GetUserByVkLogin psGetUserByVkLogin succeed");
			}

		}
	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetUserByVkLogin: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetUserByVkLogin " + std::string(e.what()));
	}


	SE::WriteToLog("TMySqlConnector::GetUserByVkLogin end");

	return result;
}

TUserListElement TMySqlConnector::GetUserByUserLogin(const std::string& userLogin)
{
	TUserListElement result;

	SE::WriteToLog("TMySqlConnector::GetUserByUserLogin begin");

	ReconnectIfNeeded();

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);

		psGetUserByUserLogin->setString(1, userLogin);

		if (psGetUserByUserLogin->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetUserByUserLogin->getResultSet());


			if (res->next())
			{
				result.login = res->getString(1);
				result.postCount = res->getInt(2);
				result.firstName = res->getString(3);
				result.lastName = res->getString(4);
				result.photoUrl = res->getString(5);
				result.photoHash = res->getString(6);

				SE::WriteToLog("TMySqlConnector::GetUserByUserLogin psGetUserByUserLogin succeed");
			}
		}
	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetUserByUserLogin: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetUserByUserLogin " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetUserByUserLogin end");

	return result;
}

void TMySqlConnector::AddComment(const std::string& username, int postId, std::string commentText)
{
	SE::WriteToLog("TMySqlConnector::AddComment begin");

	ReconnectIfNeeded();

	TUserListElement userListElement = GetUserByUserLogin(username);

	if (userListElement.login != "")
	{
		try
		{
			boost::mutex::scoped_lock lock(preparedStatementMutex);

			boost::posix_time::ptime date = boost::posix_time::second_clock::universal_time();
			std::string dateStr = boost::posix_time::to_iso_extended_string(date);

			dateStr[10] = ' ';

			psAddComment->setInt(1, postId);
			psAddComment->setString(2, commentText);
			psAddComment->setString(3, username);
			psAddComment->setString(4, dateStr);

			if (psAddComment->execute())
			{
				SE::WriteToLog("TMySqlConnector::AddComment psAddComment executed successfully");
			}

		}
		catch (sql::SQLException e)
		{
			SE::WriteToLog("Error in TMySqlConnector::AddComment: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
		}
		catch (std::exception e)
		{
			SE::WriteToLog("std::exception happened in TMySqlConnector::AddComment " + std::string(e.what()));
		}
	}

	SE::WriteToLog("TMySqlConnector::AddComment end");

}

std::vector<TCommentListElement> TMySqlConnector::GetComments(int postId)
{

	//psGetComments20
	SE::WriteToLog("TMySqlConnector::GetComments begin");

	ReconnectIfNeeded();

	std::vector<TCommentListElement> result;

	try
	{

		boost::mutex::scoped_lock lock(preparedStatementMutex);

		psGetComments20->setInt(1, postId);

		if (psGetComments20->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetComments20->getResultSet());

			while (res->next())
			{
				TCommentListElement r;

				r.CommentId = res->getInt(1);
				r.CommentText = res->getString(2);
				r.ImageUrl = res->getString(3);
				r.ImageHash = res->getString(4);
				r.UserName = res->getString(5);
				r.Date = res->getString(6);

				result.push_back(r);
			}

		}

	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetComments: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetComments " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetComments result size = " + boost::lexical_cast<std::string>(result.size()));


	SE::WriteToLog("TMySqlConnector::GetComments end");


	return result;
}

void TMySqlConnector::AddPost(const std::string& username, int feedId, std::string postHeader, std::string postText)
{
	//psAddPost
	SE::WriteToLog("TMySqlConnector::AddPost begin");

	ReconnectIfNeeded();

	TUserListElement userListElement = GetUserByUserLogin(username);

	if (userListElement.login != "")
	{
		try
		{
			boost::mutex::scoped_lock lock(preparedStatementMutex);

			boost::posix_time::ptime date = boost::posix_time::second_clock::universal_time();
			std::string dateStr = boost::posix_time::to_iso_extended_string(date);

			dateStr[10] = ' ';

			psAddPost->setInt(1, feedId);
			psAddPost->setString(2, postHeader);
			psAddPost->setString(3, postText);
			psAddPost->setString(4, username);
			psAddPost->setString(5, dateStr);

			if (psAddPost->execute())
			{
				SE::WriteToLog("TMySqlConnector::AddPost psAddPost executed successfully");
			}

		}
		catch (sql::SQLException e)
		{
			SE::WriteToLog("Error in TMySqlConnector::AddPost: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
		}
		catch (std::exception e)
		{
			SE::WriteToLog("std::exception happened in TMySqlConnector::AddPost " + std::string(e.what()));
		}

		IncreasePostCounterForUser(userListElement.login);

		SE::WriteToLog("TMySqlConnector::AddPost inner end");

	}

	SE::WriteToLog("TMySqlConnector::AddPost end");

}

void TMySqlConnector::AddPostWithImage(const std::string& username, int feedId, std::string postHeader, std::string postText, const std::string& imageUrl, const std::string& imageHash)
{
	
	SE::WriteToLog("TMySqlConnector::AddPostWithImage begin");

	ReconnectIfNeeded();

	TUserListElement userListElement = GetUserByUserLogin(username);

	if (userListElement.login != "")
	{

		try
		{
			boost::mutex::scoped_lock lock(preparedStatementMutex);

			boost::posix_time::ptime date = boost::posix_time::second_clock::universal_time();
			std::string dateStr = boost::posix_time::to_iso_extended_string(date);

			dateStr[10] = ' ';

			if (imageHash == "")
			{

				psAddPostWithImage->setInt(1, feedId);
				psAddPostWithImage->setString(2, postHeader);
				psAddPostWithImage->setString(3, postText);
				psAddPostWithImage->setString(4, username);
				psAddPostWithImage->setString(5, dateStr);
				psAddPostWithImage->setString(6, imageUrl);

				if (psAddPostWithImage->execute())
				{
					SE::WriteToLog("TMySqlConnector::AddPostWithImage psAddPostWithImage executed successfully");
				}
			}
			else
			{


				psAddPostWithImageWithHash->setInt(1, feedId);
				psAddPostWithImageWithHash->setString(2, postHeader);
				psAddPostWithImageWithHash->setString(3, postText);
				psAddPostWithImageWithHash->setString(4, username);
				psAddPostWithImageWithHash->setString(5, dateStr);
				psAddPostWithImageWithHash->setString(6, imageUrl);
				psAddPostWithImageWithHash->setString(7, imageHash);

				if (psAddPostWithImageWithHash->execute())
				{
					SE::WriteToLog("TMySqlConnector::AddPostWithImage psAddPostWithImageWithHash executed successfully");
				}
			}
		}
		catch (sql::SQLException e)
		{
			SE::WriteToLog("Error in TMySqlConnector::AddPostWithImage: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
		}
		catch (std::exception e)
		{
			SE::WriteToLog("std::exception happened in TMySqlConnector::AddPostWithImage " + std::string(e.what()));
		}

		IncreasePostCounterForUser(userListElement.login);

		SE::WriteToLog("TMySqlConnector::AddPostWithImage inner end");

	}

	SE::WriteToLog("TMySqlConnector::AddPostWithImage end");

}

void TMySqlConnector::IncreasePostCounterForUser(const std::string& username)
{
	//psIncreasePostCounterForUser
	SE::WriteToLog("TMySqlConnector::IncreasePostCounterForUser begin");

	ReconnectIfNeeded();

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);

		psIncreasePostCounterForUser->setString(1, username);

		if (psIncreasePostCounterForUser->execute())
		{
			SE::WriteToLog("TMySqlConnector::IncreasePostCounterForUser psIncreasePostCounterForUser executed successfully");
		}

	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::IncreasePostCounterForUser: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::IncreasePostCounterForUser " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::IncreasePostCounterForUser end");
}


std::vector<TComicsSeriesListElement> TMySqlConnector::GetComicsSeries()
{
	SE::WriteToLog("TMySqlConnector::GetComicsSeries begin");

	ReconnectIfNeeded();

	std::vector<TComicsSeriesListElement> result;

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);

		if (psGetComicsSeries->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetComicsSeries->getResultSet());

			while (res->next())
			{
				TComicsSeriesListElement r;
				r.seriesId = res->getInt(1);
				r.translatedTitle = res->getString(2);
				r.originalTitle = res->getString(3);
				r.description = res->getString(4);
				r.episodesCount = res->getInt(5);
				r.translatedImageUrl = res->getString(6);
				r.translatedImageHash = res->getString(7);
				r.originalImageUrl = res->getString(8);
				r.originalImageHash = res->getString(9);

				result.push_back(r);
			}

		}

	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetComicsSeriesList: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetComicsSeriesList " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetComicsSeries result size = " + boost::lexical_cast<std::string>(result.size()));


	SE::WriteToLog("TMySqlConnector::GetComicsSeries end");

	return result;
}

std::vector<TComicsEpisodeListElement> TMySqlConnector::GetComicsEpisodes(int seriesId)
{
	SE::WriteToLog("TMySqlConnector::GetComicsEpisodes begin");

	ReconnectIfNeeded();

	std::vector<TComicsEpisodeListElement> result;

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);

		psGetComicsEpisodes->setInt(1, seriesId);

		if (psGetComicsEpisodes->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetComicsEpisodes->getResultSet());
			
			while (res->next())
			{
				TComicsEpisodeListElement r;
				r.episodeId = res->getInt(1);
				r.seriesId = res->getInt(2);
				r.title = res->getString(3);
				r.description = res->getString(4);
				r.position = res->getInt(5);
				r.picturesCount = res->getInt(6);

				result.push_back(r);
			}

		}

	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetComicsEpisodesList: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetComicsEpisodesList " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetComicsEpisodes result size = " + boost::lexical_cast<std::string>(result.size()));


	SE::WriteToLog("TMySqlConnector::GetComicsEpisodes end");


	return result;
}

std::vector<TComicsPictureListElement> TMySqlConnector::GetComicsPictures(int episodeId)
{
	std::vector<TComicsPictureListElement> result;

	SE::WriteToLog("TMySqlConnector::GetComicsPictures begin");

	ReconnectIfNeeded();

	try
	{
		boost::mutex::scoped_lock lock(preparedStatementMutex);

		psGetComicsPictures->setInt(1, episodeId);

		if (psGetComicsPictures->execute())
		{
			std::shared_ptr< sql::ResultSet > res(psGetComicsPictures->getResultSet());

			while (res->next())
			{
				TComicsPictureListElement r;


				r.pictureId = res->getInt(1);
				r.episodeId = res->getInt(2);
				r.position = res->getInt(3);

				r.translatedImageUrl = res->getString(4);
				r.translatedImageHash = res->getString(5);
				r.originalImageUrl = res->getString(6);
				r.originalImageHash = res->getString(7);


				result.push_back(r);
			}

		}

	}
	catch (sql::SQLException e)
	{
		SE::WriteToLog("Error in TMySqlConnector::GetComicsPicturesList: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
	}
	catch (std::exception e)
	{
		SE::WriteToLog("std::exception happened in TMySqlConnector::GetComicsPicturesList " + std::string(e.what()));
	}

	SE::WriteToLog("TMySqlConnector::GetComicsPictures result size = " + boost::lexical_cast<std::string>(result.size()));

	SE::WriteToLog("TMySqlConnector::GetComicsPictures end");


	return result;
}

std::vector<int> TMySqlConnector::GetPurchaseListForUser(const std::string& username)
{
	SE::WriteToLog("TMySqlConnector::GetPurchaseListByUser begin");

	std::vector<int> result;


	ReconnectIfNeeded();

	TUserListElement userListElement = GetUserByUserLogin(username);

	if (userListElement.login != "")
	{
		try
		{
			boost::mutex::scoped_lock lock(preparedStatementMutex);

			psGetPurchaseListByUser->setString(1, username);

			if (psGetPurchaseListByUser->execute())
			{
				std::shared_ptr< sql::ResultSet > res(psGetPurchaseListByUser->getResultSet());

				while (res->next())
				{
					result.push_back(res->getInt(1));
				}

			}
		}
		catch (sql::SQLException e)
		{
			SE::WriteToLog("Error in TMySqlConnector::GetPurchaseListByUser: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
		}
		catch (std::exception e)
		{
			SE::WriteToLog("std::exception happened in TMySqlConnector::GetPurchaseListByUser " + std::string(e.what()));
		}

		//IncreasePostCounterForUser(userListElement.login);

		SE::WriteToLog("TMySqlConnector::GetPurchaseListByUser inner end");

	}

	SE::WriteToLog("TMySqlConnector::GetPurchaseListByUser end");

	SE::WriteToLog("TMySqlConnector::GetPurchaseListByUser result size = " + boost::lexical_cast<std::string>(result.size())+" for user: " + username);

	return result;
}


void TMySqlConnector::AddPurchaseForUser(const std::string& username, int purchaseId)
{
	SE::WriteToLog("TMySqlConnector::AddPurchaseForUser begin");

	ReconnectIfNeeded();

	TUserListElement userListElement = GetUserByUserLogin(username);

	if (userListElement.login != "")
	{

		std::vector<int> purchaseList = GetPurchaseListForUser(username);

		bool purchaseAlreadyExist = false;

		for (int finishedPurchaseId : purchaseList)
		{
			if (purchaseId == finishedPurchaseId)
			{
				purchaseAlreadyExist = true;
				SE::WriteToLog("TMySqlConnector::AddPurchaseForUser purchase Already Exist, username: " + username + " purchaseId: " + boost::lexical_cast<std::string>(purchaseId));
				break;
			}
		}

		if (!purchaseAlreadyExist)
		{

			try
			{
				boost::mutex::scoped_lock lock(preparedStatementMutex);

				boost::posix_time::ptime date = boost::posix_time::second_clock::universal_time();
				std::string dateStr = boost::posix_time::to_iso_extended_string(date);

				dateStr[10] = ' ';

				//psAddPurchaseForUser = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("INSERT INTO t_purchases (userId, purchaseId, dateTime)  VALUES(?, ?, ?)"));
				//psGetPurchaseListByUser = std::shared_ptr<sql::PreparedStatement>(Connection->prepareStatement("SELECT purchaseId FROM t_purchases WHERE userId=?"));


				psAddPurchaseForUser->setString(1, username);
				psAddPurchaseForUser->setInt(2, purchaseId);
				psAddPurchaseForUser->setString(3, dateStr);

				if (psAddPurchaseForUser->execute())
				{
					SE::WriteToLog("TMySqlConnector::AddPurchaseForUser psAddPurchaseForUser executed successfully");
				}

			}
			catch (sql::SQLException e)
			{
				SE::WriteToLog("Error in TMySqlConnector::AddPurchaseForUser: " + boost::lexical_cast<std::string>(e.getErrorCode()) + " " + e.what());
			}
			catch (std::exception e)
			{
				SE::WriteToLog("std::exception happened in TMySqlConnector::AddPurchaseForUser " + std::string(e.what()));
			}

		}

		SE::WriteToLog("TMySqlConnector::AddPurchaseForUser inner end");

	}

	SE::WriteToLog("TMySqlConnector::AddPurchaseForUser end");
}

std::string TMySqlConnector::GenerateLogin(const std::string& userId)
{
	return "vkUser" + userId;
}

#endif