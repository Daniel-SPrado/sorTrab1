#include <vector>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <mutex>

#include "renderer.h"
#include "../lib/lodepng/lodepng.h"

std::mutex mtx;

#define N 100

// Clamp double to min/max of 0/1
inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
// Clamp to between 0-255
inline int toInt(double x){ return int(clamp(x)*255+.5); }

Renderer::Renderer(Scene *scene, Camera *camera) {
    m_scene = scene;
    m_camera = camera;
    m_pixel_buffer = new Vec[m_camera->get_width()*m_camera->get_height()];
}

typedef struct thread_data{
	int w;
	int h;
	int i;
	double sr;
	int s;
	Scene *m_s;
	Camera *m_c;
	Vec *m_p_buffer;

} thread_data;

void *worker(void *arg)
{
	thread_data *tdata=(thread_data *)arg;
	int width = tdata->w;
	int height = tdata->h;
	int k = tdata->i;
	double samples_recp = tdata->sr;
	int samples = tdata->s;

	for(int y = k*(height/N);y < (k+1)*(height/N); y++)
	{
        	unsigned short Xi[3]={0,0,y};               // Stores seed for erand48

		for(int x =0; x <width;x++)
		{
        	        Vec col = Vec();
			for(int a = 0; a < samples; a++)
			{
        	        	Ray ray = tdata->m_c->get_ray(x, y, a>0, Xi);
        	        	col = col + tdata->m_s->trace_ray(ray,0,Xi);
			}
			tdata->m_p_buffer[(y)*width + x] = col * samples_recp;
		}
	}
}

void Renderer::render(int samples) {

	int width = m_camera->get_width();
	int height = m_camera->get_height();

	double samples_recp = 1./samples;

	pthread_t threads[N];
	thread_data tdata[N];


    // Main Loop
	for(int y = 0; y < N; y++){
		tdata[y].w = width;
		tdata[y].h = height;
		tdata[y].sr = samples_recp;
		tdata[y].s = samples;	
		tdata[y].m_c = m_camera;
		tdata[y].m_s = m_scene;
		tdata[y].m_p_buffer = m_pixel_buffer;
		tdata[y].i = y;
	}
	for (int y=0; y<N; y++)
		pthread_create(&threads[y], NULL, worker, (void *)&tdata[y]);
	for(int k= 0; k<N;k++){
	        fprintf(stderr, "\rRendering (%i samples): %.2f%% ",      // Prints
	                samples, (double)(k+1)/N*100);                   // progress
		pthread_join(threads[k], NULL);
	}
}

void Renderer::save_image(const char *file_path) {
    int width = m_camera->get_width();
    int height = m_camera->get_height();

    std::vector<unsigned char> pixel_buffer;

    int pixel_count = width*height;
 //   thread_data tdata;
 //   m_pixel_buffer = tdata.m_p_buffer;

    for (int i=0; i<pixel_count; i++) {
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].x));
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].y));
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].z));
        pixel_buffer.push_back(255);
    }

    //Encode the image
    unsigned error = lodepng::encode(file_path, pixel_buffer, width, height);
    //if there's an error, display it
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

    pixel_buffer.clear();
}
