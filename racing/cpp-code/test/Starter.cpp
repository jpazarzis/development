
#include "Starter.h"
#include <queue>

using namespace std;

#define MAX_NUMBER_OF_STARTERS 200
#define MAX_BUFFER_SIZE 10000


static char BUFFER[MAX_NUMBER_OF_STARTERS][MAX_BUFFER_SIZE];

static queue<char*> buffer_queue;

Starter Starter::Make(FILE* file)
{
    Starter s;

    if(fgets(s._buffer, MAX_BUFFER_SIZE, file) == NULL) {
        s.ResetBuffer();
    }

    return s;
}

char* Starter::c_str()
{
    return _buffer;
}

bool Starter::IsValid() const
{
    return 0 != _buffer;
}

void Starter::Initialize()
{
    for(register int i = 0; i < MAX_NUMBER_OF_STARTERS; ++i) {
        buffer_queue.push(BUFFER[i]);
    }
}


void Starter::ResetBuffer()
{
    if(0 != _buffer) {
        buffer_queue.push(_buffer);
        _buffer = 0;
    }
}


Starter::Starter() : _buffer(0)
{
    if(!buffer_queue.empty()) {
        _buffer = buffer_queue.front();
        buffer_queue.pop();
    }
}

Starter::~Starter()
{
}


