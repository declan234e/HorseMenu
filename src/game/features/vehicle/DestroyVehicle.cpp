#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "game/rdr/Natives.hpp"

namespace YimMenu::Features
{
	class DestroyVehicle : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			if (Self::GetVehicle())
			{
				// I broke the goddamn wheel
				VEHICLE::_BREAK_OFF_VEHICLE_WHEEL(Self::GetVehicle().GetHandle(), 0);
				VEHICLE::_BREAK_OFF_VEHICLE_WHEEL(Self::GetVehicle().GetHandle(), 1);
			}
				
		}
	};

	static DestroyVehicle _DestroyVehicle{"destroyvehicle", "Destroy Vehicle", "Destroys your current vehicle"};
}