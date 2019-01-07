#include "ProximityList.h"
#include <set>

namespace VEProximity
{

	ProximityList::ProximityList(veci3 _size)
	{
		size = _size;
		count = size.area();
		shellCount = size.shellSize();

		veci3 start = (size / 2) * -1;
		veci3 v;

		std::set<veci3, Veci3MagnitudeCompare> order;

		for (int x = 0; x < size.x; ++x)
		{
			v.x = x + start.x;
			for (int y = 0; y < size.y; ++y)
			{
				v.y = y + start.y;
				for (int z = 0; z < size.z; ++z)
				{
					v.z = z + start.z;
					order.insert(v);
				}
			}
		}

		list = new veci3[size.area()];
		shell = new veci3[size.shellSize()];
		int i = 0;
		int s = 0;
		for(auto it = order.begin(); it != order.end(); ++it)
		{
			list[i++] = *it;
			auto relPos = *it - start;
			if (size.isOnShell(relPos))
			{
				shell[s++] = *it;
			}
		}
	}


	ProximityList::~ProximityList()
	{
		if(count > 0)
			delete[] list;
		if(shellCount > 0)
			delete[] shell;
	}

	bool ProximityList::next(veci3 & v)
	{
		if (cursor < count)
		{
			v = list[cursor++];
			return true;
		}
		return false;
	}

	bool ProximityList::nextShell(veci3 & v)
	{
		if (shellCursor < shellCount)
		{
			v = shell[shellCursor++];
			return true;
		}
		return false;
	}


}
