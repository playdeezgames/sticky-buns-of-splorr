#pragma once
#include "worlddata.h"
#include <string>
#include "framebuffercellcolor.h"
#include "messagedata.h"
class Message
{
private:
    WorldData& _data;
    size_t _index;
    Message();
    MessageData& GetMessageData() {return _data.GetMessage(_index);}
    const MessageData& GetMessageData() const{return _data.GetMessage(_index);}
public:
    Message(WorldData& data, size_t index):_data(data),_index(index){}
    const std::string& GetText() const;
    FrameBufferCellColor GetForeground() const;
    FrameBufferCellColor GetBackground() const;
};