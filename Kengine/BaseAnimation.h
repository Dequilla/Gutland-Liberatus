#pragma once

#include <sstream>
#include <string>

namespace Kengine
{
class SpriteSheet;
using Frame = unsigned int;

class BaseAnimation
{
    friend class SpriteSheet;
public:
                    BaseAnimation();
    virtual         ~BaseAnimation();

    // Setters
    void            SetSpriteSheet(SpriteSheet* sheet);
    void            SetFrame(Frame frame);
    void            SetStartFrame(Frame frame);
    void            SetEndFrame(Frame frame);
    void            SetFrameRow(Frame row);
    void            SetActionStart(Frame frame);
    void            SetActionEnd(Frame frame);
    void            SetFrameTime(float time);
    void            SetLooping(bool loop);
    void            SetName(const std::string& name);

    // Getters
    SpriteSheet*    GetSpriteSheet();
    Frame           GetFrame();
    Frame           GetStartFrame();
    Frame           GetEndFrame();
    Frame           GetFrameRow();
    int             GetActionStart();
    int             GetActionEnd();
    float           GetFrameTime();
    float           GetElapsedTime();
    bool            IsLooping();
    bool            IsPlaying();
    bool            IsInAction();
    std::string     GetName();

    void            Play();
    void            Pause();
    void            Stop();
    void            Reset();

    virtual void Update(const float& dt);

    friend std::stringstream& operator >> (std::stringstream & stream, BaseAnimation & a)
    {
        a.ReadIn(stream);
        return stream;
    }

protected:
    virtual void    FrameStep()                       = 0;
    virtual void    CropSprite()                      = 0;
    virtual void    ReadIn(std::stringstream& stream) = 0;

    Frame           m_frameCurrent;
    Frame           m_frameStart;
    Frame           m_frameEnd;
    Frame           m_frameRow;

    int             m_frameActionStart; // Frame when a specific "action" begins
    int             m_frameActionEnd;   // Frame when a specific "action" ends
    float           m_frameTime;
    float           m_elapsedTime;
    bool            m_loop;
    bool            m_playing;
    std::string     m_name;
    SpriteSheet     *m_spriteSheet;
};
}
