#pragma once
#include "solopch.h"
#include "Solo/Core/TimeStep.h"
#include <al.h>
#include <alc.h>
#include <efx.h>
#include <EFX-Util.h>

#include <vendor/glm/glm/glm.hpp>

#include <vendor/audiofile/AudioFile.h>
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)
#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)


namespace Solo {
class AudioEngine
{
public:



	AudioEngine()
	{
		device = nullptr;
		source = 0;
		buffer = 0;
		context = nullptr;
		contextMadeCurrent = false;
		state = 0;

		AudioFile<double> test = AudioFile<double>("C:\\Users\\quaz9\\source\\repos\\Solo_Engine\\Solo_Editor\\assets\\sounds\\test_mono.wav");
		test.load("C:\\Users\\quaz9\\source\\repos\\Solo_Engine\\Solo_Editor\\assets\\sounds\\test_mono.wav");

		device = alcOpenDevice(nullptr);
		if (!device) DisplayALError("DEVICE NOT FOUND", 0);



		context = nullptr;
		if (!alcCall(alcCreateContext, context, device, device, nullptr) || !context)
		{
			std::cerr << "ERROR: Could not create audio context" << std::endl;
			/* probably exit program */
		}

		contextMadeCurrent = false;
		if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, device, context)
			|| contextMadeCurrent != ALC_TRUE)
		{
			std::cerr << "ERROR: Could not make audio context current" << std::endl;
			/* probably exit or give up on having sound */
		}


		alCall(alGenBuffers, 1, &buffer);

		std::uint8_t channels;
		std::int32_t sampleRate;
		std::uint8_t bitsPerSample;
		ALsizei dataSize;
		char* rawSound = load_wav("C:\\Users\\quaz9\\source\\repos\\Solo_Engine\\Solo_Editor\\assets\\sounds\\test_mono.wav", channels, sampleRate, bitsPerSample, dataSize);
		if (!rawSound)
		{
			std::cerr << "ERROR: Could not load wav" << std::endl;

		}
		char* end = rawSound + dataSize;
		std::vector<char> soundData(rawSound, end);

		ALenum format;
		if (channels == 1 && bitsPerSample == 8)
			format = AL_FORMAT_MONO8;
		else if (channels == 1 && bitsPerSample == 16)
			format = AL_FORMAT_MONO16;
		else if (channels == 2 && bitsPerSample == 8)
			format = AL_FORMAT_STEREO8;
		else if (channels == 2 && bitsPerSample == 16)
			format = AL_FORMAT_STEREO16;
		else
		{
			std::cerr
				<< "ERROR: unrecognised wave format: "
				<< channels << " channels, "
				<< bitsPerSample << " bps" << std::endl;
		}


		alCall(alBufferData, buffer, format, soundData.data(), soundData.size(), sampleRate);
		soundData.clear(); // erase the sound in RAM

		alCall(alDistanceModel, AL_EXPONENT_DISTANCE);
		alCall(alListenerf, AL_GAIN, 1);

		alCall(alListener3f,AL_POSITION, 0.0, 0.0, 0.0);


		alCall(alGenSources, 1, &source);
		alCall(alSourcef, source, AL_PITCH, 1);
		alCall(alSourcef, source, AL_GAIN, 1.f);
		alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
		alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
		alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
		alCall(alSourcei, source, AL_BUFFER, buffer);

		alCall(alSourcePlay, source);

		state = AL_PLAYING;


	}



	static void DisplayALError(const char* msg, ALenum error) {
		std::cerr << msg << " " << error << std::endl;
	}

	static void Init() {
		if (!sInstance)
		{
			sInstance = new AudioEngine();
		}
	
		
	};


	static void Update(TimeStep dt)
	{
		if (sInstance)
		{
			sInstance->_update(dt);
		}
	};


	static void SetListenerPos(glm::vec3 pos)
	{
		if (sInstance)
		{
			sInstance->_setListenerPos(pos);
		}
	};

	static void SetListenerOrientation(glm::vec2 pos)
	{
		if (sInstance)
		{
			sInstance->_setListenerOrientation(pos);
		}
	};

	static void Shutdown()
	{

		if (sInstance)
		{
			sInstance->_shutdown();
		}

	};



	static bool check_al_errors(const std::string& filename, const std::uint_fast32_t line)
	{
		ALenum error = alGetError();
		if (error != AL_NO_ERROR)
		{
			std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
			switch (error)
			{
			case AL_INVALID_NAME:
				std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
				break;
			case AL_INVALID_ENUM:
				std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
				break;
			case AL_INVALID_VALUE:
				std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
				break;
			case AL_INVALID_OPERATION:
				std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
				break;
			case AL_OUT_OF_MEMORY:
				std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
				break;
			default:
				std::cerr << "UNKNOWN AL ERROR: " << error;
			}
			std::cerr << std::endl;
			return false;
		}
		return true;
	}






	static std::int32_t convert_to_int(char* buffer, std::size_t len)
	{
		std::int32_t a = 0;
		if (std::endian::native == std::endian::little)
			std::memcpy(&a, buffer, len);
		else
			for (std::size_t i = 0; i < len; ++i)
				reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
		return a;
	}

	static bool load_wav_file_header(std::ifstream& file,
		std::uint8_t& channels,
		std::int32_t& sampleRate,
		std::uint8_t& bitsPerSample,
		ALsizei& size)
	{
		char buffer[4];
		if (!file.is_open())
			return false;

		// the RIFF
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read RIFF" << std::endl;
			return false;
		}
		if (std::strncmp(buffer, "RIFF", 4) != 0)
		{
			std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
			return false;
		}

		// the size of the file
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read size of file" << std::endl;
			return false;
		}

		// the WAVE
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read WAVE" << std::endl;
			return false;
		}
		if (std::strncmp(buffer, "WAVE", 4) != 0)
		{
			std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
			return false;
		}

		// "fmt/0"
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read fmt/0" << std::endl;
			return false;
		}

		// this is always 16, the size of the fmt data chunk
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read the 16" << std::endl;
			return false;
		}

		// PCM should be 1?
		if (!file.read(buffer, 2))
		{
			std::cerr << "ERROR: could not read PCM" << std::endl;
			return false;
		}

		// the number of channels
		if (!file.read(buffer, 2))
		{
			std::cerr << "ERROR: could not read number of channels" << std::endl;
			return false;
		}
		channels = convert_to_int(buffer, 2);

		// sample rate
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read sample rate" << std::endl;
			return false;
		}
		sampleRate = convert_to_int(buffer, 4);

		// (sampleRate * bitsPerSample * channels) / 8
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
			return false;
		}

		// ?? dafaq
		if (!file.read(buffer, 2))
		{
			std::cerr << "ERROR: could not read dafaq" << std::endl;
			return false;
		}

		// bitsPerSample
		if (!file.read(buffer, 2))
		{
			std::cerr << "ERROR: could not read bits per sample" << std::endl;
			return false;
		}
		bitsPerSample = convert_to_int(buffer, 2);

		// data chunk header "data"
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read data chunk header" << std::endl;
			return false;
		}
		if (std::strncmp(buffer, "data", 4) != 0)
		{
			std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
			return false;
		}

		// size of data
		if (!file.read(buffer, 4))
		{
			std::cerr << "ERROR: could not read data size" << std::endl;
			return false;
		}
		size = convert_to_int(buffer, 4);

		/* cannot be at the end of file */
		if (file.eof())
		{
			std::cerr << "ERROR: reached EOF on the file" << std::endl;
			return false;
		}
		if (file.fail())
		{
			std::cerr << "ERROR: fail state set on the file" << std::endl;
			return false;
		}

		return true;
	}

	static char* load_wav(const std::string& filename,
		std::uint8_t& channels,
		std::int32_t& sampleRate,
		std::uint8_t& bitsPerSample,
		ALsizei& size)
	{
		std::ifstream in(filename, std::ios::binary);
		if (!in.is_open())
		{
			std::cerr << "ERROR: Could not open \"" << filename << "\"" << std::endl;
			return nullptr;
		}
		if (!load_wav_file_header(in, channels, sampleRate, bitsPerSample, size))
		{
			std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"" << std::endl;
			return nullptr;
		}

		char* data = new char[size];

		in.read(data, size);

		return data;
	}






		static bool check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
		{
 			ALCenum error = alcGetError(device);
			if (error != ALC_NO_ERROR)
			{
				std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
				switch (error)
				{
				case ALC_INVALID_VALUE:
					std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
					break;
				case ALC_INVALID_DEVICE:
					std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
					break;
				case ALC_INVALID_CONTEXT:
					std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
					break;
				case ALC_INVALID_ENUM:
					std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
					break;
				case ALC_OUT_OF_MEMORY:
					std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
					break;
				default:
					std::cerr << "UNKNOWN ALC ERROR: " << error;
				}
				std::cerr << std::endl;
				return false;
			}
			return true;
		}


		template<typename alFunction, typename... Params>
		static auto alCallImpl(const char* filename,
			const std::uint_fast32_t line,
			alFunction function,
			Params... params)
			-> typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
		{
			auto ret = function(std::forward<Params>(params)...);
			check_al_errors(filename, line);
			return ret;
		}

		template<typename alFunction, typename... Params>
		static auto alCallImpl(const char* filename,
			const std::uint_fast32_t line,
			alFunction function,
			Params... params)
			-> typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
		{
			function(std::forward<Params>(params)...);
			return check_al_errors(filename, line);
		}

		template<typename alcFunction, typename... Params>
		static auto alcCallImpl(const char* filename,
			const std::uint_fast32_t line,
			alcFunction function,
			ALCdevice* device,
			Params... params)
			-> typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
		{
			function(std::forward<Params>(params)...);
			return check_alc_errors(filename, line, device);
		}

		template<typename alcFunction, typename ReturnType, typename... Params>
		static auto alcCallImpl(const char* filename,
			const std::uint_fast32_t line,
			alcFunction function,
			ReturnType& returnValue,
			ALCdevice* device,
			Params... params)
			-> typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>
		{
			returnValue = function(std::forward<Params>(params)...);
			return check_alc_errors(filename, line, device);
		}

		static bool get_available_devices(std::vector<std::string>& devicesVec, ALCdevice* device)
		{
			const ALCchar* devices;
			if (!alcCall(alcGetString, devices, device, nullptr, ALC_DEVICE_SPECIFIER))
				return false;

			const char* ptr = devices;

			devicesVec.clear();

			do
			{
				devicesVec.push_back(std::string(ptr));
				ptr += devicesVec.back().size() + 1;
			} while (!(*(ptr + 1) == '\0' and *(ptr + 2) == '\0'));

			return true;
		}


	private:
		static AudioEngine* sInstance;
		// TODO LMAO no
		ALCdevice* device;
		ALuint source;
		ALuint buffer;
		ALCcontext* context;
		ALCboolean contextMadeCurrent;
		ALint state;

		void _update(TimeStep dt) {
			
			
			//alCall(alSource3f, source, AL_POSITION, floor(posX), 0, 0);
			//alCall(alSource3f, source, AL_VELOCITY, velX, 0, 0);
			if (state == AL_PLAYING)
			{
				//alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
			}
			else
			{
				//Shutdown();
			}
		}

		void _shutdown() {
			alCall(alDeleteSources, 1, &source);
			alCall(alDeleteBuffers, 1, &buffer);

			alcCall(alcMakeContextCurrent, contextMadeCurrent, device, nullptr);
			alcCall(alcDestroyContext, device, context);

			ALCboolean closed;
			alcCall(alcCloseDevice, closed, device, device);
		};


		void _setListenerPos(glm::vec3 pos)
		{
			alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		};

		void _setListenerOrientation(glm::vec2 dir)
		{
			float orientation[6] = {};
			orientation[0] = -sinf(dir[1]*3.14f/180.f);
			orientation[1] = 0;
			orientation[2] = -cosf(dir[1] * 3.14f / 180.0f);
			orientation[3] = 0;
			orientation[4] = 1;
			orientation[5] = 0;

			alListenerfv(AL_ORIENTATION, orientation);
		};


	};


}

