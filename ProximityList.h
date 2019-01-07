#pragma once

#include "MathHelper.h"

namespace VEProximity
{

	struct Veci3MagnitudeCompare
	{
		bool operator() (const veci3& lhs, const veci3& rhs) const
		{
			int lmag = lhs.magnitudeSq();
			int rmag = rhs.magnitudeSq();
			if (lmag == rmag)
			{
				return lhs.hash() < rhs.hash(); // tie-breaker
			}
			return lmag < rmag;
		}
	};

	class ProximityList
	{
	public:
		ProximityList(veci3 _size);
		~ProximityList();

		bool next(veci3& v);
		void reset() { cursor = 0; }

		bool nextShell(veci3& v);
		void resetShell() { shellCursor = 0; }

		bool next(bool full, veci3& v)
		{
			if (full) return next(v);
			else return nextShell(v);
		}

		void reset(bool full)
		{
			if (full) reset();
			else resetShell();
		}

	private:
		veci3 size;
		int count;
		veci3 *list;

		int shellCount;
		veci3 *shell;

		int cursor;
		int shellCursor;
	};
}
