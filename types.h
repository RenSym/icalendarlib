#ifndef _TYPES_H
#define _TYPES_H

#include <string>
#include <list>
#include "date.h"

using namespace std;

struct DeleteItem {
	template <typename T>
	void operator()(T *ptr) { delete ptr; }
};

// Custom data types

typedef enum { VCALENDAR, VEVENT, VALARM } Component;
typedef enum { DISPLAY=0, PROCEDURE, AUDIO, EMAIL } AlarmAction;

struct Recurrence {
	Recurrence(): Freq(YEAR), Interval(0), Count(0) {}
	operator string() const;
	bool IsEmpty() const { return (Interval == 0); }
	void Clear() { Interval = 0; }
	
	TimeUnit Freq;
	unsigned short Interval, Count;
	Date Until;
};

struct AlarmTrigger {
	AlarmTrigger(): Before(true), Value(0), Unit(MINUTE) {}
	AlarmTrigger &operator =(const string &Text);
	operator string() const;
	
	bool Before;
	unsigned short Value;
	TimeUnit Unit;
};

struct Alarm {
	Alarm(): Active(false), Action(DISPLAY) {}
	operator string() const;
	void Clear() {
		Description.clear();
	}
	
	bool Active;
	AlarmAction Action;
	AlarmTrigger Trigger;
	string Description;
};

struct Event {
	Event(): Alarms(new list<Alarm>), RecurrenceNo(0), BaseEvent(this) {}
	Event(const Event &Base):
		UID(Base.UID),
		Summary(Base.Summary),
		Description(Base.Description),
		Categories(Base.Categories),
		DtStamp(Base.DtStamp),
		DtStart(Base.DtStart),
		DtEnd(Base.DtEnd),
        TimezoneStart(Base.TimezoneStart),
        TimezoneEnd(Base.TimezoneEnd),
		RRule(Base.RRule),
		Alarms(Base.Alarms),
		RecurrenceNo(Base.RecurrenceNo)
	{
		BaseEvent = Base.BaseEvent == (Event *)&Base ? (Event *)&Base : Base.BaseEvent;
	}
	~Event() {
		if (BaseEvent == this)
			delete Alarms;
	}
	operator string() const;
	bool HasAlarm(const Date &From, const Date &To);

	string UID, Summary, Description, Categories, TimezoneStart, TimezoneEnd;
	Date DtStamp, DtStart, DtEnd;
	Recurrence RRule;
	list<Alarm> *Alarms;
	unsigned short RecurrenceNo;
	Event *BaseEvent;
};

struct EventsCriteria {
	EventsCriteria(): AllEvents(false), IncludeRecurrent(true) {}
	
	Date From, To;
	bool AllEvents, IncludeRecurrent;
};

inline ostream &operator <<(ostream &stream, const Recurrence &RRule) {
	stream << RRule.operator string();
	return stream;
}

inline ostream &operator <<(ostream &stream, const Alarm &Alarm) {
	stream << Alarm.operator string();
	return stream;
}

inline ostream &operator <<(ostream &stream, const Event &Event) {
	stream << Event.operator string();
	return stream;
}

#endif // _TYPES_H
