////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <time.h>

#ifndef MSG_FILE_H
#define MSG_FILE_H

typedef struct {
	int id;
	time_t second;
	int msecond;
	short type;	
}MSG_YX;

#define MSG_SIZE 200

const short MSG_TYPE_BREAKER = 0;	//开关跳闸类型
const short MSG_TYPE_TOTAL = 1;		//事故总动作类型
const short MSG_TYPE_PROTECT = 2;	//保护动作类型

/*memory map
int current_pos
YX_MSG	\
YX_MSG	|
......	|
......	| message bodys
......	|
YX_MSG	/
*/

class CFaultMsg
{
public:
	CFaultMsg();
	~CFaultMsg();

	char *msg_file;

protected:
	bool msg_is_open;

public:
	bool msg_insert(MSG_YX* yx_msg);
	bool msg_open();
	bool msg_is_opened();
	bool msg_close();
	bool msg_select(int* id,int id_count,int gap,time_t current_second,int current_msecond);
	bool msg_delete(int id);
	bool msg_getat(int pos,MSG_YX &msg);
};

#endif //MSG_FILE_H
