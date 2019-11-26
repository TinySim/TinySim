#include <iostream>
#include "WString.h"
using namespace std;

class TL_Serial{
private:
	int begin_value;
public:
	TL_Serial();
	void begin(int);
	void print(char val);
	void print(int val);
	void print(long val);
	void print(double val);
	void print(const char* val);
	void print(const String &val);
    // -------------------------
    void println(char val);
	void println(int val);
	void println(long val);
	void println(double val);
	void println(const char* val);
	void println(const String &val);
};