#include "report.hpp"

Report::Report()
{
	this->userid = "";
}

void Report::setUserId(string userId)
{
	this->userid = userId;
}

bool Report::loggedIn()
{
	if (this->userid.length() > 0) {
		return true;
	} else {
		return false;
	}
}

void Report::recQuestions()
{

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::ResultSet *subRes;
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		stmt = con->createStatement();
		
		string searchTerm = "";

		if (this->loggedIn()) {
			subRes = stmt->executeQuery("SELECT tags FROM tblUserTags WHERE id = '" + this->userid + "' AND deleted=0");
			
			while(subRes->next()){
				searchTerm = subRes->getString("tags");
			}
			
		}

		res = stmt->executeQuery("SELECT tblQuestion.id, tblQuestion.questionTitle, tblContent.utimestamp FROM tblQuestion JOIN tblContent ON tblQuestion.contentId = tblContent.id WHERE tblQuestion.tags LIKE '%" + searchTerm + "%' AND tblQuestion.deleted = 0 ORDER BY tblContent.utimestamp DESC LIMIT 10");

		cout << "<table class=\"table table-hover\">";
		cout << "<tr>";
		cout << "<th>Question Title</th>";
		cout << "<th>Date posted</th>";
		cout << "</tr>";
		while(res->next()){
			cout << "<tr>";
			cout << "<td><a href=\"/question.html?id=" << res->getString("id") << "\">" << res->getString("questionTitle") << "</a></td>";
			cout << "<td>" << res->getString("utimestamp") << "</td>";
			cout << "</tr>";
		}
		cout << "</table>";

		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}
}

void Report::topUsers()
{

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();
		
		
		res = stmt->executeQuery("SELECT tblUser.username, tblUser.displayname, tblUserReputation.reputation, tblUser.creationdate FROM tblUser JOIN tblUserReputation ON tblUser.username = tblUserReputation.username WHERE tblUser.deleted = 0  ORDER BY tblUserReputation.reputation DESC LIMIT 10;");

		cout << "<table class=\"table table-hover\">";
		cout << "<tr>";
		cout << "<th>Username</th>";
		cout << "<th>Reputation</th>";
		cout << "<th>Creation Date</th>";
		cout << "</tr>";
		while(res->next()){
			cout << "<tr>";
			cout << "<td><a href=\"/profile.html?username=" << res->getString("username") << "\">" << res->getString("displayname") << "</td>";
			cout << "<td>" << res->getString("reputation") << "</td>";
			cout << "<td>" << res->getString("creationdate") << "</td>";
			cout << "</tr>";
		}
		cout << "</table>";

		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}
}

void Report::featured()
{

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();
		
		
		res = stmt->executeQuery("SELECT tblQuestion.id, tblQuestion.questionTitle FROM BitQA.tblFeatured JOIN tblQuestion ON tblFeatured.id = tblQuestion.id WHERE tblQuestion.deleted=0;");

		cout << "<table class=\"table table-hover\">";
		cout << "<tr>";
		cout << "<th>Question Title</th>";
		cout << "</tr>";
		while(res->next()){
			cout << "<tr>";
			cout << "<td><a href=\"/question.html?id=" << res->getString("id") << "\">" << res->getString("questionTitle") << "</a></td>";
			cout << "</tr>";
		}
		cout << "</table>";

		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}
}

void Report::topQuestions()
{

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();

		
		res = stmt->executeQuery("SELECT Q.id, Q.questionTitle, C.upvotes FROM tblQuestion Q JOIN tblContent C ON Q.contentId = C.id WHERE Q.deleted = 0 ORDER BY C.upvotes DESC LIMIT 10;");

		cout << "<table class=\"table table-hover\">";
		cout << "<tr>";
		cout << "<th>Question Title</th>";
		cout << "<th>Upvotes</th>";
		cout << "</tr>";
		while(res->next()){
			cout << "<tr>";
			cout << "<td><a href=\"/question.html?id=" << res->getString("id") << "\">" << res->getString("questionTitle") << "</a></td>";
			cout << "<td>" << res->getString("upvotes") << "</td>";
			cout << "</tr>";
		}
		cout << "</table>";

		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}

}

void Report::activityGraph()
{
	
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		string temp = "";
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();
		
		vector<string> results;
		
		/*
		 * Questions
		 */
		res = stmt->executeQuery("SELECT count(*) AS \"count\" FROM BitQA.tblQuestion WHERE qowner = '" + this->userid + "' AND deleted=0;");
		
		while(res->next()) {
			temp  = res->getString("count");
		}
		
		results.push_back(temp);
		
		/*
		 * Answers
		 */
		res = stmt->executeQuery("SELECT count(*) AS \"count\" FROM BitQA.tblAnswer WHERE aowner = '" + this->userid + "' AND deleted=0;");
		
		while(res->next()) {
			temp  = res->getString("count");
		}
		
		results.push_back(temp);
		
		/*
		 * Comments
		 */
		res = stmt->executeQuery("SELECT count(*) AS \"count\" FROM BitQA.tblComment WHERE aowner = '" + this->userid + "' AND deleted=0;");
		
		while(res->next()) {
			temp  = res->getString("count");
		}
		
		results.push_back(temp);
		
		string finalArray;
		
		for (int i = 0; i < results.size(); i++) {
			finalArray += results[i] + (i < results.size()-1? ", ": "");
		}
		
		cout << "<script>var datas = datas || {}; var datas = [" << finalArray << "];</script>" << endl;
		
		cout << "<div style=\"width:500px\"><canvas style=\"width: 500px; height: 250px;\" id=\"myChart\" width=\"400\" height=\"400\"></canvas></div>" << endl;
		cout << "<script src=\"/includes/javascript/report.js\"></script>";
		
		delete res;
		delete stmt;
		delete con;
		
	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}
}

void Report::topTags()
{

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();
		
		
		res = stmt->executeQuery("SELECT tags, count(*) AS frequency FROM tblQuestion WHERE deleted = 0 AND qowner = '" + this->userid + "' GROUP BY tags ORDER BY count(*) DESC LIMIT 5;");

		cout << "<table class=\"table table-hover\" id=\"topTagTbl\">";
		cout << "<thead>";
		cout << "<tr>";
		cout << "<th>Tags</th>";
		cout << "<th>Frequency</th>";
		cout << "</tr>";
		cout << "</thead>";
		
		int i = 0;
		
		while(res->next()){
			
			string tagz = res->getString("tags");
			tagz.erase(std::remove(tagz.begin(), tagz.end(), '<'), tagz.end());
			tagz.erase(std::remove(tagz.begin(), tagz.end(), '>'), tagz.end());
			
			cout << "<tr>";
			cout << "<td>" << tagz << "</td>";
			cout << "<td>" << res->getString("frequency") << "</td>";
			cout << "</tr>";
			i++;
		}
		
		if (i <= 0) {
			cout << "<tr>";
			cout << "<td>N/A</td>";
			cout << "<td>N/A</td>";
			cout << "</tr>";
		}
		
		cout << "</table>";

		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}

}

void Report::topPostedQuestions()
{
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();
		
		
		res = stmt->executeQuery("SELECT Q.id, Q.questionTitle, C.upvotes FROM tblQuestion Q JOIN tblContent C ON Q.contentId = C.id WHERE Q.qowner = '" + this->userid + "' AND Q.deleted=0 ORDER BY C.upvotes DESC LIMIT 10;");
		
		cout << "<table class=\"table table-hover\" id=\"topQuesTbl\">";
		cout << "<thead>";
		cout << "<tr>";
		cout << "<th>Question Title</th>";
		cout << "<th>Upvotes</th>";
		cout << "</tr>";
		cout <<"</thead>";
		
		int i = 0;
		
		while(res->next()){
			cout << "<tr>";
			cout << "<td><a href=\"/question.html?id=" << res->getString("id") << "\">" << res->getString("questionTitle") << "</a></td>";
			cout << "<td>" << res->getString("upvotes") << "</td>";
			cout << "</tr>";
			
			i++;
		}
		
		if (i <= 0) {
			cout << "<tr>";
			cout << "<td>N/A</td>";
			cout << "<td>N/A</td>";
			cout << "</tr>";
		}
		
		cout << "</table>";
		
		delete res;
		delete stmt;
		delete con;
		
	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}
}

void Report::allUsers(){
	
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();
		
		
		res = stmt->executeQuery("SELECT tblUser.username, tblUser.displayname, tblUserReputation.reputation, tblUser.creationdate FROM tblUser JOIN tblUserReputation ON tblUser.username = tblUserReputation.username WHERE tblUser.deleted = 0  ORDER BY tblUserReputation.reputation desc");
		
		cout << "<table class=\"table table-hover\" id=\"allUserTable\">";
		cout << "<thead>";
		cout << "<tr>";
		cout << "<th>Username</th>";
		cout << "<th>Reputation</th>";
		cout << "<th>Creation Date</th>";
		cout << "<th></th>";
		cout << "<th></th>";
		cout << "<th></th>";
		cout << "</tr>";
		cout << "</thead>";
		while(res->next()){
			cout << "<tr>";
			cout << "<td><a href=\"/profile.html?username=" << res->getString("username") << "\">" << res->getString("username") << "</td>";
			cout << "<td>" << res->getString("reputation") << "</td>";
			cout << "<td>" << res->getString("creationdate") << "</td>";
			
			
			cout << "<td> <a href=\"/changerep.html?username="
				 << res->getString("username") <<"\" class=\"btn btn-info\"> Change rep pts </a> </td>";
			
			cout << "<td> <a href=\"/adminedit.html?username="
				 << res->getString("username") << "\" class=\"btn btn-warning\"> Edit </a> </td>";
			
			cout << "<td> <a href=\"/admindeactivate.html?username="
				 << res->getString("username")
				 <<"\" class=\"btn btn-danger\"> Deactivate </a> </td>";
			cout << "</tr>";
		}
		cout << "</table>";
		
		delete res;
		delete stmt;
		delete con;
		
	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}
}

void Report::topPostedAnswers()
{
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();
		
		
		res = stmt->executeQuery("SELECT A.id, C.content, C.upvotes FROM tblAnswer A JOIN tblContent C ON A.contentId = C.id WHERE A.aowner = '" + this->userid + "' AND A.deleted=0 ORDER BY C.upvotes DESC LIMIT 10;");
		
		cout << "<table class=\"table table-hover\" id=\"topAnsTbl\">";
		cout << "<thead>";
		cout << "<tr>";
		cout << "<th>Title</th>";
		cout << "<th>Upvotes</th>";
		cout << "</tr>";
		cout << "</thead>";
		
		int i = 0;
		
		while(res->next()){
			cout << "<tr>";
			cout << "<td>" << res->getString("content") << "</a></td>";
			cout << "<td>" << res->getString("upvotes") << "</td>";
			cout << "</tr>";
			
			i++;
		}
		
		if (i <= 0) {
			cout << "<tr>";
			cout << "<td>N/A</td>";
			cout << "<td>N/A</td>";
			cout << "</tr>";
		}
		
		cout << "</table>";
		
		delete res;
		delete stmt;
		delete con;
		
	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}
}

void Report::postHistory()
{

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		
		
		driver = get_driver_instance();
		con = driver->connect(BitQA::Database::HOST,
							  BitQA::Database::USERNAME,
							  BitQA::Database::PASSWORD
							  );
		
		con->setSchema(BitQA::Database::SCHEMA);
		
		stmt = con->createStatement();
		
		res = stmt->executeQuery("SELECT tblQuestion.id, tblQuestion.questionTitle, tblContent.utimestamp FROM BitQA.tblQuestion JOIN tblContent ON tblQuestion.contentId = tblContent.id WHERE tblQuestion.qowner = '" + this->userid + "' AND tblQuestion.deleted=0 ORDER BY tblContent.utimestamp DESC LIMIT 10;");

		cout << "<table class=\"table table-hover\" id=\"topPostingHistoryTbl\">";
		cout << "<thead>";
		cout << "<tr>";
		cout << "<th>Question</th>";
		cout << "<th>Timestamp</th>";
		cout << "</tr>";
		cout << "</thead>";
		
		
		int i = 0;
		
		while(res->next()){
			cout << "<tr>";
			cout << "<td><a href=\"/question.html?id=" << res->getString("id") << "\">" << res->getString("questionTitle") << "</a></td>";
			cout << "<td>" << res->getString("utimestamp") << "</td>";
			cout << "</tr>";
			i++;
		}
		
		if (i <= 0) {
			cout << "<tr>";
			cout << "<td>None Posted</td>";
			cout << "<td>None Posted</td>";
			cout << "</tr>";
		}
		
		cout << "</table>";

		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "<p>Error</p>";
	}

}
