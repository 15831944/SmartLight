// TestLight01.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "LightModel.h"
#include "F:\Gkpro\ldns\code\Include\curl\curl.h"
#include "F:\Gkpro\ldns\code\Include\json\json.h"
#include "F:\Gkpro\ldns\code\rtdb\include\gSystemDef.h"
#include "F:\Gkpro\ldns\code\Include\commFrameInclude\Q_HttpLink.h"
#include "F:\Gkpro\ldns\code\Include\tinyxmlInclude\tinyxmlinclude.h"
#include "F:\Gkpro\ldns\code\Include\tinyxmlInclude\tinyxmlinclude.h"
#include "F:\Gkpro\ldns\code\rtdb\include\gSystemDef.h"
#include "F:\Gkpro\ldns\code\plungins\CommHttpQHCGQ\Q_ProtocolDBQH.h"
#include <math.h>
//�Ƶ�����
bool LightControl::handleFrameLight(string url)
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) 
	{
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, url);

		// ���ý������ݵĴ������ʹ�ű���
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, funGetLight);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);

	}
	return true;
}
//�Ƶ����ݷ���
size_t LightControl::funcGetLight(void *strptr, size_t size, size_t nmemb, void *stream)
{
	Json::Reader reader;
	Json::Value root;

	std::string strSQL;
	strSQL = (char *)strptr;//���յ�����

	if (reader.parse(strSQL, root))//��json������root
	{
		std::string strHour;//ʱ���
		std::string stradd;//�豸��ַ
		printf("root size=%d\n", root.size());


		for (int i = 0; i<root.size(); i++)
		{
			strHour = root["status"][i]["ts"].asString();

			printf("datetime=%s\n", strHour.c_str());
			stradd = root["status"][i]["addr"].asString;
			std::string trigger = root["status"][i]["triggerBy"].asString();
			std::string v = root["status"][i]["v"].asString();
			std::string lastupdate = root["lastUpdate"].asString();
			//printf("get current Energy =%f,power=%f,addr=%d\n", dE, dPower, hex_to_decimal(strMac.c_str(), strMac.size()));





		}
	}

	return 0;
}




