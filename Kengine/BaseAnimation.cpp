#include "BaseAnimation.h"
#include "SpriteSheet.h"

namespace Kengine
{
BaseAnimation::BaseAnimation() : m_frameCurrent(0), m_frameStart(0),
m_frameEnd(0), m_frameRow(0), m_frameTime(0.0f), m_elapsedTime(0.0f),
m_frameActionStart(-1), m_frameActionEnd(-1), m_loop(false), m_playing(false)
{
}

BaseAnimation::~BaseAnimation()
{

}

void BaseAnimation::SetSpriteSheet(SpriteSheet* sheet)
{
	m_spriteSheet = sheet;
}

void BaseAnimation::SetFrame(Frame frame)
{
	if ((frame >= m_frameStart && frame <= m_frameEnd) ||
		(frame >= m_frameEnd && frame <= m_frameStart))
	{
		m_frameCurrent = frame;
	}
}

void BaseAnimation::SetStartFrame(Frame frame)
{
	m_frameStart = frame;
}

void BaseAnimation::SetEndFrame(Frame frame)
{
	m_frameEnd = frame;
}

void BaseAnimation::SetFrameRow(Frame row)
{
	m_frameRow = row;
}

void BaseAnimation::SetActionStart(Frame frame)
{
	m_frameActionStart = frame;
}

void BaseAnimation::SetActionEnd(Frame frame)
{
	m_frameActionEnd = frame;
}

void BaseAnimation::SetFrameTime(float time)
{
	m_frameTime = time;
}

void BaseAnimation::SetLooping(bool loop)
{
	m_loop = loop;
}

void BaseAnimation::SetName(const std::string & name)
{
	m_name = name;
}

SpriteSheet* BaseAnimation::GetSpriteSheet()
{
	return m_spriteSheet;
}

Frame BaseAnimation::GetFrame()
{
	return m_frameCurrent;
}

Frame BaseAnimation::GetStartFrame()
{
	return m_frameStart;
}

Frame BaseAnimation::GetEndFrame()
{
	return m_frameEnd;
}

Frame BaseAnimation::GetFrameRow()
{
	return m_frameRow;
}

int BaseAnimation::GetActionStart()
{
	return m_frameActionStart;
}

int BaseAnimation::GetActionEnd()
{
	return m_frameActionEnd;
}

float BaseAnimation::GetFrameTime()
{
	return m_frameTime;
}

float BaseAnimation::GetElapsedTime()
{
	return m_elapsedTime;
}

bool BaseAnimation::IsLooping()
{
	return m_loop;
}

bool BaseAnimation::IsPlaying()
{
	return m_playing;
}

bool BaseAnimation::IsInAction()
{
	// If any of the values are -1 the action is always performed.
	if (m_frameActionStart == -1 || m_frameActionEnd == -1)
	{
		return true;
	}

	return (m_frameCurrent >= m_frameActionStart &&
			m_frameCurrent <= m_frameActionEnd);
}

std::string BaseAnimation::GetName()
{
	return std::string();
}

void BaseAnimation::Play()
{
	m_playing = true;
}

void BaseAnimation::Pause()
{
	m_playing = false;
}

void BaseAnimation::Stop()
{
	m_playing = false;
	Reset();
}

void BaseAnimation::Reset()
{
	m_frameCurrent = m_frameStart;
	m_elapsedTime = 0.0f;
	CropSprite();
}

void BaseAnimation::Update(const float& dt)
{
	if (!m_playing) { return; }
	m_elapsedTime += dt;
	if (m_elapsedTime < m_frameTime) { return; }
	FrameStep();
	CropSprite();
	m_elapsedTime = 0;
}
}