#include <bits/stdc++.h>
using namespace std;

vector<string> ReadFileLines(string path) {
	vector<string> lines;

	fstream file_handler(path.c_str());

	if (file_handler.fail()) {
		cout << "\n\nERROR: Can't open the file\n\n";
		return lines;
	}
	string line;

	while (getline(file_handler, line)) {
		if (line.size() == 0)
			continue;
		lines.push_back(line);
	}

	file_handler.close();
	return lines;
}

void WriteFileLines(string path, vector<string> lines, bool append = true) {
	auto status = ios::in | ios::out | ios::app;

	if (!append)
		status = ios::in | ios::out | ios::trunc;	// overwrite

	fstream file_handler(path.c_str(), status);

	if (file_handler.fail()) {
		cout << "\nERROR: File can\'t be opened !\n";
		return;
	}
	for (auto line : lines)
		file_handler << line << "\n";

	file_handler.close();
}

vector<string> SplitString(string s, string del = ",") {
	vector<string> strs;

	int pos = 0;
	string substr;
	//Noha,Ahmed,Mohamed
	while ((pos = (int)s.find(del)) != -1) {
		//A
		//,
		substr = s.substr(0, pos);
		strs.push_back(substr);
		s.erase(0, pos + del.length());
		//Ahmed,Mohamed
		//Mohamed
	}
	strs.push_back(s);
	return strs;
}

int ToInt(string str) {
	istringstream iss(str);
	int num;
	iss >> num;

	return num;
}

int ReadInt(int low, int high) {
	cout << "\nEnter number between " << low << " - " << high << ": ";
	int value;

	cin >> value;

	if (low <= value && value <= high)
		return value;
	//==================

	cout << "ERROR: invalid number...Try again\n";
	return ReadInt(low, high);
}

int MainMenu() {
	cout << "\nMenu:\n";
	cout << "1- Print Questions To Me\n2- Print Questions From Me\n3- Answer Question\n4- Delete Question\n5- Ask Question\n6- List System Users\n7- Feed\n8- Logout";

	return ReadInt(1, 8);

}

int LogMenu() {
	cout << "\nMenu:\n";
	cout << "1- Login\n2- SignUp";

	return ReadInt(1, 2);

}

struct Question {
	int questionId;

	int parentQuestionId;
	int fromUserId;
	int toUserId;
	int isAnonymous;
	string questionText;
	string answerText;

	Question() {
		questionId = parentQuestionId = fromUserId = toUserId = -1;
		isAnonymous = 1;
	}

	Question(string line) {
		vector<string> substrs = SplitString(line);
		assert(substrs.size() == 7);

		questionId = ToInt(substrs[0]);
		parentQuestionId = ToInt(substrs[1]);
		fromUserId = ToInt(substrs[2]);
		toUserId = ToInt(substrs[3]);
		isAnonymous = ToInt(substrs[4]);
		questionText = substrs[5];
		answerText = substrs[6];
	}

	string ToString() {
		ostringstream oss;
		oss << questionId << "," << parentQuestionId << "," << fromUserId << "," << toUserId << "," << isAnonymous << "," << questionText << "," << answerText;

		return oss.str();
	}

	void PrintToQuestion() {
		string prefix = "";

		if (parentQuestionId != -1)
			prefix = "\tThread: ";

		cout << prefix << "Question Id (" << questionId << ")";
		if (!isAnonymous)
			cout << " from user id(" << fromUserId << ")";

		cout << "\t Question: " << questionText << "\n";

		if (answerText != "")
			cout << prefix << "\tAnswer: " << answerText << "\n";
		cout << "\n";
	}

	void PrintFromQuestion() {
		cout << "Question Id (" << questionId << ")";
		if (!isAnonymous)
			cout << " !AQ";

		cout << " to user id(" << toUserId << ")";
		cout << "\t Question: " << questionText;

		if (answerText != "")
			cout << "\tAnswer: " << answerText << "\n";
		else
			cout << "\tNOT Answered YET\n";
	}

	void PrintFeedQuestion() {
		if (parentQuestionId != -1)
			cout << "Thread Parent Question ID (" << parentQuestionId << ") ";

		cout << "Question Id (" << questionId << ")";
		if (!isAnonymous)
			cout << " from user id(" << fromUserId << ")";

		cout << " To user id(" << toUserId << ")";

		cout << "\t Question: " << questionText << "\n";
		if (answerText != "")
			cout << "\tAnswer: " << answerText << "\n";
	}

};

struct User {
	int userID;
	string username;
	string password;
	string name;
	string email;
	int allowAnonymous;

	vector<int> sentQuestionIds;

	map<int, vector<int>> questionid_questionidsThead_to_map;

	User() {
		userID = allowAnonymous = -1;
	}

	User(string line) {
		vector<string> substrs = SplitString(line);
		assert(substrs.size() == 6);

		userID = ToInt(substrs[0]);
		username = substrs[1];
		password = substrs[2];
		name = substrs[3];
		email = substrs[4];
		allowAnonymous = ToInt(substrs[5]);
	}

	string ToString() {
		ostringstream oss;
		oss << userID << "," << username << "," << password << "," << name << "," << email << "," << allowAnonymous;

		return oss.str();
	}

	void Print() {
		cout << "User " << userID << ", " << username << " " << password << ", " << name << ", " << email << "\n";
	}
};

struct UsersManager {
	map<string, User> userame_userobject_map;
	//ssm ,USer
	User currentUser;
	int lastID;

	UsersManager() {
		lastID = 0;
	}

	void LoadDatabase() {
		lastID = 0;
		userame_userobject_map.clear();

		vector<string> lines = ReadFileLines("18_users.txt");
		for (auto& line : lines) {
			User user(line);
			userame_userobject_map[user.username] = user;
			lastID = max(lastID, user.userID);
		}
	}

	void AccessSystem() {
		int choice = LogMenu();
		if (choice == 1)
			SignIn();
		else
			SignUp();
	}

	void SignIn() {
		LoadDatabase();

		while (true) {
			cout << "Enter user name & password: ";
			cin >> currentUser.username >> currentUser.password;

			if (!userame_userobject_map.count(currentUser.username)) {
				cout << "\nInvalid user name or password. Try again\n\n";
				continue;
			}
			User user_exist = userame_userobject_map[currentUser.username];

			if (currentUser.password != user_exist.password) {
				cout << "\nInvalid user name or password. Try again\n\n";
				continue;
			}
			currentUser = user_exist;
			break;
		}
	}

	void SignUp() {
		while (true) {
			cout << "Enter user name. (No spaces): ";
			cin >> currentUser.username;

			if (userame_userobject_map.count(currentUser.username))
				cout << "Already used. Try again\n";
			else
				break;
		}
		cout << "Enter password: ";
		cin >> currentUser.password;

		cout << "Enter name: ";
		cin >> currentUser.name;

		cout << "Enter email: ";
		cin >> currentUser.email;

		cout << "Allow anonymous questions? (0 or 1): ";
		cin >> currentUser.allowAnonymous;

		currentUser.userID = ++lastID;
		userame_userobject_map[currentUser.username] = currentUser;

		UpdateTextFiles(currentUser);
	}

	void ListUserIds() {
		for (auto& pair : userame_userobject_map)
			cout << "ID: " << pair.second.userID << "\t\tName: " << pair.second.name << "\n";
	}

	pair<int, int> ReadUserId() {
		int userID;
		cout << "Enter User id or -1 to cancel: ";
		cin >> userID;

		if (userID == -1)
			return make_pair(-1, -1);

		for (auto& pair : userame_userobject_map) {
			if (pair.second.userID == userID)
				return make_pair(userID, pair.second.allowAnonymous);
		}

		cout << "Invalid User ID. Try again\n";
		return ReadUserId();
	}

	void UpdateTextFiles(User& user) {
		string line = user.ToString();
		vector<string> lines(1, line);
		WriteFileLines("18_users.txt", lines);
	}
};
struct QuestionsManager {

	map<int, vector<int>> questionid_questionidsThead_to_map;



	map<int, Question> questionid_questionobject_map;

	int lastID;

	QuestionsManager() {
		lastID = 0;
	}

	void LoadDatabase() {
		lastID = 0;
		questionid_questionidsThead_to_map.clear();
		questionid_questionobject_map.clear();

		vector<string> lines = ReadFileLines("18_questions.txt");
		for (auto& line : lines) {
			Question question(line);
			lastID = max(lastID, question.questionId);

			questionid_questionobject_map[question.questionId] = question;

			if (question.parentQuestionId == -1)
				questionid_questionidsThead_to_map[question.questionId].push_back(question.questionId);
			else
				questionid_questionidsThead_to_map[question.parentQuestionId].push_back(question.questionId);
		}
	}

	void FillUserQuestions(User& user) {
		user.sentQuestionIds.clear();
		user.questionid_questionidsThead_to_map.clear();

		for (auto& pair : questionid_questionidsThead_to_map) {	// pair<int, vector<Question>>
			for (auto& questionId : pair.second) {		//  vector<Question>

				// Get the question from the map. & means same in memory, DON'T COPY
				Question& question = questionid_questionobject_map[questionId];

				if (question.fromUserId == user.userID)
					user.sentQuestionIds.push_back(question.questionId);

				if (question.toUserId == user.userID) {
					if (question.parentQuestionId == -1)
						user.questionid_questionidsThead_to_map[question.questionId].push_back(question.questionId);
					else
						user.questionid_questionidsThead_to_map[question.parentQuestionId].push_back(question.questionId);
				}
			}
		}
	}

	void PrintUserToQuestions(User& user) {
		cout << "\n";

		if (user.questionid_questionidsThead_to_map.size() == 0)
			cout << "No Questions";

		for (auto& pair : user.questionid_questionidsThead_to_map) {
			for (auto& questionId : pair.second) {

				Question& question = questionid_questionobject_map[questionId];
				question.PrintToQuestion();
			}
		}
		cout << "\n";
	}

	void PrintUserFromQuestions(User& user) {
		cout << "\n";
		if (user.sentQuestionIds.size() == 0)
			cout << "No Questions";

		for (auto& questionId : user.sentQuestionIds) {


			Question& question = questionid_questionobject_map[questionId];
			question.PrintFromQuestion();
		}
		cout << "\n";
	}




	int ReadQuestionIdAny(User& user) {
		int questionId;
		cout << "Enter Question id or -1 to cancel: ";
		cin >> questionId;

		if (questionId == -1)
			return -1;

		if (!questionid_questionobject_map.count(questionId)) {
			cout << "\nERROR: No question with such ID. Try again\n\n";
			return ReadQuestionIdAny(user);
		}
		Question& question = questionid_questionobject_map[questionId];

		if (question.toUserId != user.userID) {
			cout << "\nERROR: Invalid question ID. Try again\n\n";
			return ReadQuestionIdAny(user);
		}
		return questionId;
	}

	int ReadQuestionIdThread(User& user) {
		int questionId;
		cout << "For thread question: Enter Question id or -1 for new question: ";
		cin >> questionId;

		if (questionId == -1)
			return -1;

		if (!questionid_questionidsThead_to_map.count(questionId)) {
			cout << "No thread question with such ID. Try again\n";
			return ReadQuestionIdThread(user);
		}
		return questionId;
	}






	void AnswerQuestion(User& user) {
		int questionId = ReadQuestionIdAny(user);

		if (questionId == -1)
			return;

		Question& question = questionid_questionobject_map[questionId];

		question.PrintToQuestion();

		if (question.answerText != "")
			cout << "\nWarning: Already answered. Answer will be updated\n";

		cout << "Enter answer: ";
		getline(cin, question.answerText);
		getline(cin, question.answerText);
	}

	void DeleteQuestion(User& user) {
		int questionId = ReadQuestionIdAny(user);

		if (questionId == -1)
			return;

		vector<int> ids_to_remove;


		if (questionid_questionidsThead_to_map.count(questionId)) {
			ids_to_remove = questionid_questionidsThead_to_map[questionId];
			questionid_questionidsThead_to_map.erase(questionId);
		}
		else {
			ids_to_remove.push_back(questionId);


			for (auto& pair : questionid_questionidsThead_to_map) {
				vector<int>& vec = pair.second;
				for (int pos = 0; pos < (int)vec.size(); ++pos) {
					if (questionId == vec[pos]) {
						vec.erase(vec.begin() + pos);
						break;
					}
				}
			}

		}

		for (auto id : ids_to_remove) {
			questionid_questionobject_map.erase(id);
		}
	}

	void AskQuestion(User& user, pair<int, int> toUserPair) {
		Question question;

		if (!toUserPair.second) {
			cout << "Note: Anonymous questions are not allowed for this user\n";
			question.isAnonymous = 0;
		}
		else {
			cout << "Is anonymous questions?: (0 or 1): ";
			cin >> question.isAnonymous;
		}

		question.parentQuestionId = ReadQuestionIdThread(user);

		cout << "Enter question text: ";
		getline(cin, question.questionText);
		getline(cin, question.questionText);

		question.fromUserId = user.userID;
		question.toUserId = toUserPair.first;


		question.questionId = ++lastID;

		questionid_questionobject_map[question.questionId] = question;

		if (question.parentQuestionId == -1)
			questionid_questionidsThead_to_map[question.questionId].push_back(question.questionId);
		else
			questionid_questionidsThead_to_map[question.parentQuestionId].push_back(question.questionId);
	}

	void ListFeed() {
		for (auto& pair : questionid_questionobject_map) {
			Question& question = pair.second;

			if (question.answerText == "")
				continue;

			question.PrintFeedQuestion();
		}
	}

	void UpdateTextFiles() {
		vector<string> lines;

		for (auto& pair : questionid_questionobject_map)
			lines.push_back(pair.second.ToString());

		WriteFileLines("18_questions.txt", lines, false);
	}
};


struct AskMeSystem {
	UsersManager usersManager;
	QuestionsManager questionManager;

	void LoadDatabase(bool fillQuest = false) {
		usersManager.LoadDatabase();
		questionManager.LoadDatabase();

		if (fillQuest)
			questionManager.FillUserQuestions(usersManager.currentUser);
	}

	void run() {
		LoadDatabase();
		usersManager.AccessSystem();
		questionManager.FillUserQuestions(usersManager.currentUser);



		while (true) {
			int choice = MainMenu();
			LoadDatabase(true);

			switch (choice)
			{
			case 1:
				questionManager.PrintUserToQuestions(usersManager.currentUser);
				break;
			case 2:
				questionManager.PrintUserFromQuestions(usersManager.currentUser);

				break;
			case 3:
				questionManager.AnswerQuestion(usersManager.currentUser);
				questionManager.UpdateTextFiles();
				break;
			case 4:
				questionManager.DeleteQuestion(usersManager.currentUser);
				questionManager.FillUserQuestions(usersManager.currentUser);
				questionManager.UpdateTextFiles();
				break;
			case 5: {
				pair<int, int> toUserPair = usersManager.ReadUserId();
				if (toUserPair.first != -1) {
					questionManager.AskQuestion(usersManager.currentUser, toUserPair);
					questionManager.UpdateTextFiles();
				}
				break;
			}
			case 6:
				usersManager.ListUserIds();
				break;
			case 7:
				questionManager.ListFeed();
				break;
			default:
				break;
			}



		}


		run();
	}
};

int main() {
	AskMeSystem service;
	service.run();

	return 0;
}
