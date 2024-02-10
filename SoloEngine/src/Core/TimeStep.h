#pragma once

namespace Solo {

	class TimeStep
	{
	public:
		TimeStep(long long time = 0)
			:timeNS_(time)
		{

		}

		operator float() const { return static_cast<float>(timeNS_); }
		operator double() const { return static_cast<double>(timeNS_); }

		double GetSeconds() const { return static_cast<double>(timeNS_) * 1e-9; }

		double GetMilli() const { return static_cast<double>(timeNS_) * 1e-6; }

		double GetMicro() const { return static_cast<double>(timeNS_) * 1e-3; }

		double GetNano() const { return static_cast<double>(timeNS_); }


	private:
		long long timeNS_;

	};

}