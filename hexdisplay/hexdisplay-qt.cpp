/*

	Hexdisplay gui

	- create 7 segment hex display widget
	- load simulation memory mapped file
	- periodically check memory mapped file for segment data

									*/	
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <QtGui>
#include "sharedmem.h"
#include "rasterwindow.h"

class SevenSegmentDisplayWindow : public RasterWindow
{
public:
    SevenSegmentDisplayWindow();

    void setSegment(int segment, int value);
	void setMemoryMap(void* map);

protected:
    void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;
    void render(QPainter *p) Q_DECL_OVERRIDE;

private:
    int m_timerId;
    int segments[8];
	struct SharedMemoryLayout* shm;		
};


SevenSegmentDisplayWindow::SevenSegmentDisplayWindow()
{
    setTitle("Seven Segment Display Driver");
    resize(300, 300);

    for (int i = 0; i < 8; i++)
    {
	segments[i] = 1;
    }

    m_timerId = startTimer(100);

	shm = NULL;
}

void SevenSegmentDisplayWindow::setSegment(int segment, int value)
{
    if (segment >= 0 && segment < 8)
   	 segments[segment] = value; 
}

void SevenSegmentDisplayWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId)
    {

		if (shm != NULL)
		{

			if (shm->requestUIExit == 1)
				close();	

			for (int i = 0; i < 8; i ++)
				setSegment(i,shm->sevenSegmentData[i]);

		}

        renderLater();
    }
}

void SevenSegmentDisplayWindow::setMemoryMap(void* map)
{
	shm = (struct SharedMemoryLayout*)map;
}

void SevenSegmentDisplayWindow::render(QPainter *p)
{

    static const QPoint horizontalSegment[4] = {
        QPoint(-30, -5),
        QPoint(30, -5),
        QPoint(30, 5),
	QPoint(-30, 5)
    };

    static const QPoint verticalSegment[4] = {
        QPoint(-45, -2),
        QPoint(-35, -2),
        QPoint(-35, -70),
	QPoint(-45, -70)
    };

    QColor segmentColorLit(255, 69, 0);
    QColor segmentColorUnLit(69, 69, 69, 50);

    p->setRenderHint(QPainter::Antialiasing);
    p->translate(width() / 2, height() / 2);

    int side = qMin(width(), height());
    p->scale(side / 300.0, side / 300.0);

    p->setPen(Qt::NoPen);
    p->setBrush(segmentColorUnLit);

    struct penOp {
	int x;
	int y;
	int segment;
	int horizontal;
    };

    struct penOp penOps[7] = {{0,0,6,true},{0,0,5,false},
			      {0,-70,0,true},
			      {0,70,3,true},
			      {0,70,4,false},
			      {80,0,1,false},
			      {80,70,2,false}};

    for (int i = 0; i < 7; i ++)
    {
    	
	p->save();

	//printf("penOps segment: %d value: %d \n", 
	//		penOps[i].segment, segments[penOps[i].segment]);
    	
	if (segments[penOps[i].segment] == 0)
    		p->setBrush(segmentColorLit);

	p->translate(penOps[i].x,penOps[i].y);
    	p->drawConvexPolygon(penOps[i].horizontal ? 
				horizontalSegment : verticalSegment, 4); // c
    	p->restore(); 
    }

}

int main(int argc, char **argv)
{
    int retval;
	int fd;
	const char *shmFname = SHAREDMEMORYFILE; 
	void *map;
	struct SharedMemoryLayout* shm;

	// open shared memory file

	fd = open(shmFname, O_RDWR);
    if (fd == -1) 
	{
		perror("Error opening file for reading");
		exit(EXIT_FAILURE);
    }

    map = mmap(0, SHAREDMEMORYSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) 
	{
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
    }

 
	QGuiApplication app(argc, argv);
	
    SevenSegmentDisplayWindow display;
	display.setMemoryMap(map);   
 
	display.show();

    retval = app.exec();

	// post exit signal

	printf("Exiting UI...\n");
	fflush(0);

	shm = (struct SharedMemoryLayout*)map;
	shm->uiExited = 1;

	// munmap

    if (munmap(map, SHAREDMEMORYSIZE) == -1) {
		perror("Error un-mmapping the file");
    }

	// exit
	return retval;
}
