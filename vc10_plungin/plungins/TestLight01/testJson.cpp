#include "F:\Gkpro\ldns\code\Include\json\json.h"
using namespace std;
int main()
{
	const char* str =

		"{\"praenomen\":\"Gaius\",\"nomen\":\"Julius\",\"cognomen\":\"Caezar\","

		"\"born\":-100,\"died\":-44}";



	Json::Reader reader;

	Json::Value root;



	//从字符串中读取数据

	if (reader.parse(str, root))

	{

		string praenomen = root["praenomen"].asString();

		string nomen = root["nomen"].asString();

		string cognomen = root["cognomen"].asString();

		int born = root["born"].asInt();

		int died = root["died"].asInt();



		cout << praenomen + " " + nomen + " " + cognomen

			<< " was born in year " << born

			<< ", died in year " << died << endl;

	}



	return 0;


}