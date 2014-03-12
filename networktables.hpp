#ifndef NETWORKTABLES_HPP
#define NETWORKTABLES_HPP

#if defined (__cplusplus)
extern "C" {
#endif
	#include <jni.h>
#if defined (__cplusplus)
}
#endif

#include <pthread.h>
#include <vector>
#include <iostream>
#include <boost/variant/variant.hpp>
#include <string>

using namespace std;
using namespace boost;

typedef variant<bool, int, long, double> generic_primitive;
typedef vector<generic_primitive> generic_packet;

class com_module
{
	private:
		JavaVM * jvm;
		JNIEnv * env;
		jclass * class_networktables;

		vector<generic_primitive> packet_out;

		int mode;
		double dx;
		double dy;
		double theta;

		static string ip;

		bool has_updated;
		bool enabled;
		pthread_t com_thread;

	public:
		com_module(JavaVM*);
		~com_module();

		static void * com_thread_entry_func(void *);
		static com_module get_instance(const string&);
		static JNIEnv * create_vm(JavaVM **);


		inline void enable() {enabled = true;}
		inline void disable() {enabled = false;}

		inline pthread_t * get_thread() {return &com_thread;}
		inline double get_dx() {return dx;}
		inline double get_dy() {return dy;}
		inline double get_theta() {return theta;}
		inline int get_mode() {return mode;}

		bool reset();

		void get_packet(vector<generic_primitive>&);

		void init_cpp();
		void init_java(const jstring&);
		void run_java();
		void update_dx();
		void update_dy();
		void update_theta();
		void update_mode();

		void set_out_element(const int&, const generic_primitive&, const int&);
		void update_packet(const generic_packet&);
		void set_packet(const generic_packet&);
	protected:
		void run();

};



#endif
