
typedef void (*MessageCallback)(unsigned char event_id, unsigned int color, char* text);

#define MESSAGE_TYPE_TEXT 1

#define MESSAGE_COLOR_GREEN 0x2589

class ControlBase {
  public:
    ControlBase() {
      msg = 0;
    }
    void SetMessageCallback(MessageCallback msgPtr) {
      msg = msgPtr;
    }
    void Message(unsigned char event_id, unsigned int color, char* text) {
      if(msg != 0) {
        msg(event_id, color, text);
      }
    }
    virtual const char* GetId() = 0;
    virtual void Init() = 0;
    virtual void Connected() = 0;
    virtual void Disconnected() = 0;
    virtual void Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy) = 0;
    virtual void Idle() = 0;
    
  private:
    MessageCallback msg;  
    
};
