#ifndef CONDITION_H
#define CONDITION_H

class Media;


class Condition
{
	public:
		virtual ~Condition() = default;
		virtual bool match(Media &media) const = 0;
};

#endif // CONDITION_H
