#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

#include <QThread>

#include <Vision.hpp>
#include <Network/Sender.hpp>
#include <Network/Network.hpp>

uint64_t timestamp()
{
	struct timeval time;
	gettimeofday(&time, 0);
	
	return time.tv_sec * 1000000 + time.tv_usec;
}

class VisionSender : public QThread
{
	public:
		VisionSender(float sx, float sy, unsigned int fps)
		{
			_id = _nextId++;
			_sx = sx;
			_sy = sy;
			
			_fps = fps;
			
			printf("Creating fake camera with id: %d\n", _id);
			printf("Robot start @ (%f , %f) w/ fps: %d\n", _sx, _sy, _fps);
		}
		
	protected:
		void run()
		{
			Network::Sender sender(Network::Address, Network::Vision);
				
			Packet::Vision packet;
			packet.camera = _id;
			packet.blue.push_back(Packet::Vision::Robot());
	
			const int msecs = (int)(1000/_fps);
			
			float rad = 0;
			
			//loop and send out robot positions
			while (true)
			{
				packet.timestamp = timestamp();
				Packet::Vision::Robot& r = packet.blue[0];
			
				rad += .025;
				
				if (rad > 2*M_PI)
				{
					rad -= 2*M_PI;
				}
				
				r.shell = _id;
				r.pos.x = _sx + cos(rad);
				r.pos.y = _sy + sin(rad);
				r.angle = 45.0f * _id;
				
				sender.send(packet);
				
				//printf("[%d] %lld :: Pos: %.3f %.3f\n", _id, packet.timestamp, r.pos.x, r.pos.y);
				
				QThread::msleep(msecs);
			}
		}
		
	private:
		unsigned int _id;
		
		float _sx, _sy;
		
		unsigned int _fps;
		
		static unsigned int _nextId;
};

unsigned int VisionSender::_nextId = 0;

int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <cam count> <fps>\n", argv[0]);
		return 0;
	}
	
	int cameras = atoi(argv[1]);
	unsigned int fps = atoi(argv[2]);
	
	if (cameras < 1)
	{
		cameras = 1; 
	}
	
	float x = 2.0f;
	float y = 0;
	
	float inc = 4.0f;
	
	if (cameras > 1)
	{
		inc /= (cameras-1);
	}
	
	VisionSender sender1(x, y, fps);
	sender1.start();
	
	for (int i=1 ; i<cameras ; ++i)
	{
		usleep(12340);
		
		x -= inc;
		VisionSender* sender = new VisionSender(x, y, fps);
		sender->start();
	}
	
	printf("---------------------------------------------------------\n");
	
	printf("Sending data...\n");
		
	sender1.wait();
	
	return 0;
}
