#if defined (__cplusplus)
extern "C" {
#endif
	#include <jni.h>
#if defined (__cplusplus)
}
#endif

#include "networktables.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

using namespace std;
using namespace boost;

typedef variant<bool, int, long, double> generic_primitive;
const int FORMAT[] = {};

string com_module::ip;

com_module::com_module(JavaVM * jvm_arg)
{
//	printf("Initializing thread.\n");
	jvm = jvm_arg;
	has_updated = true;
}

com_module::~com_module()
{
	delete class_networktables;
	jvm->DestroyJavaVM();
}

void com_module::init_cpp()
{
	cout << "Finding class." << endl;
	class_networktables = new jclass;
	cout << env << endl;
	(*class_networktables) = env->FindClass("NetworkTablesModule");
	cout << "Did the thing." << endl;
	if (class_networktables == 0)
	{
		fprintf(stderr, "Couldn't find class: NetworkTablesModule\n");
	}
	else
	{
//		cout << "Found class: NetworkTables" << endl;
	}
}

void com_module::init_java(const jstring& jip)
{
	jmethodID method_init = env->GetStaticMethodID(*class_networktables, "init", "(Ljava/lang/String;)V");
	if (method_init == 0)
	{
		fprintf(stderr, "Could not locate method: init\n");
	}
	else
	{
//		printf("Found method: init\n");
	}
	env->CallStaticVoidMethod(*class_networktables, method_init, jip);
}

void com_module::run_java()
{
	jmethodID method_run = env->GetStaticMethodID(*class_networktables, "run", "()V");
	if (method_run == 0)
	{
		fprintf(stderr, "Could not find method: run");
	}
	else
	{
//		cout << "Found method: run" << endl;
	}
	env->CallStaticVoidMethod(*class_networktables, method_run);
}
void com_module::update_dx()
{
	jmethodID method_get_dx = env->GetStaticMethodID(*class_networktables, "get_dx", "()D");
	if (method_get_dx == 0)
	{
		fprintf(stderr, "Could not find method: get_dx\n");
	}
	else
	{
//		cout << "Found method: get_dx" << endl;
	}
	dx = ((double)env->CallStaticDoubleMethod(*class_networktables, method_get_dx));
//	cout << "Grabbed: dx = " << dx << endl;
}

void com_module::update_dy()
{
	jmethodID method_get_dy = env->GetStaticMethodID(*class_networktables, "get_dy", "()D");
	if (method_get_dy == 0)
	{
		fprintf(stderr, "Could not find method: get_dy\n");
	}
	else
	{
//		cout << "Found method: get_dy" << endl;
	}
	dy = ((double)env->CallStaticDoubleMethod(*class_networktables, method_get_dy));
//	cout << "Grabbed: dy = " << dy << endl;
}

void com_module::update_theta()
{
	jmethodID method_get_theta = env->GetStaticMethodID(*class_networktables, "get_angle", "()D");
	if (method_get_theta == 0)
	{
		fprintf(stderr, "Could not find method: get_angle\n");
	}
	else
	{
//		cout << "Found method: get_angle" << endl;
	}
	theta = ((double)env->CallStaticDoubleMethod(*class_networktables, method_get_theta));
//	cout << "Grabbed: theta = " << theta << endl;
}

void com_module::update_mode()
{
	jmethodID method_get_mode = env->GetStaticMethodID(*class_networktables, "get_mode", "()I");
	if (method_get_mode == 0)
	{
		fprintf(stderr, "Could not find method: get_mode\n");
	}
	else
	{
//		cout << "Found method: get_mode" << endl;
	}
	mode = ((int)env->CallStaticIntMethod(*class_networktables, method_get_mode));
//	cout << "Grabbed: mode = " << mode << endl;
}

void com_module::set_out_element(const int& type, const generic_primitive& value, const int& index )
{
	cout << "Setting Element" << endl;
	jmethodID set_bool = env->GetStaticMethodID(*class_networktables, "add_bool", "(ZI)V");
	if (set_bool == 0)
	{
		fprintf(stderr, "Could not find method: set_bool\n");
	}
	jmethodID set_int = env->GetStaticMethodID(*class_networktables, "add_int", "(II)V");
	if (set_int == 0)
	{
		fprintf(stderr, "Could not find method: set_int\n");
	}
	jmethodID set_double = env->GetStaticMethodID(*class_networktables, "add_double", "(DI)V");
	if (set_double == 0)
	{
		fprintf(stderr, "Could not find method: set_double\n");
	}
	switch(type)
	{
		case 0:
//			cout << "Setting bool." << endl;
			env->CallStaticVoidMethod(*class_networktables, set_bool,get<bool>(value), index);
			break;
		case 1:
//			cout << "Setting int." << endl;
			env->CallStaticVoidMethod(*class_networktables, set_int, get<int>(value), index);
			break;
		case 2:
//			cout << "Setting double." << endl;
			env->CallStaticVoidMethod(*class_networktables, set_double, get<double>(value), index);
		default:
			break;
	}
}

void com_module::update_packet(const generic_packet& packet)
{
	cout << "Packet updating." << endl;
	jmethodID init_packet = env->GetStaticMethodID(*class_networktables, "initialize_packet", "(I)V");
	if (init_packet == 0)
	{
		fprintf(stderr, "Could not find method: initialize_packet\n");
	}
	env->CallStaticVoidMethod(*class_networktables, init_packet, packet.size());
	cout << "Initialized packet." << endl;

	vector<jint> pack(packet.size());
	set_out_element(1, get<int>(packet.at(0)), 0);

	for (int i = 1; i < packet.size(); i += 3)
	{
		set_out_element(2, get<double>(packet.at(i)), i);
		set_out_element(2, get<double>(packet.at(i+1)), i+1);
		set_out_element(1, get<int>(packet.at(i+2)), i+2);
	}
}

void com_module::set_packet(const generic_packet& pack)
{
	cout << "setting packet: " << get<int>(pack[0]) << endl;
	packet_out = pack;
	has_updated = false;
}

bool com_module::reset()
{
	return (pthread_create(&com_thread, NULL, com_thread_entry_func, this) == 0);
}

void * com_module::com_thread_entry_func(void * context)
{
	printf("Entering Thread.\n");
	((com_module *)context)->run();
	return NULL;
}

void com_module::get_packet(generic_packet& packet_in)
{
	generic_packet packet(4);
	packet[0] = get_mode();
	packet[1] = get_dx();
	packet[2] = get_dy();
	packet[3] = get_theta();
	packet_in = packet;
}

void com_module::run()
{
	cout << "Attaching to JVM. " << jvm << endl;
	(jvm)->AttachCurrentThread((void **)&env, NULL);
	init_cpp();
	init_java(env->NewStringUTF(ip.c_str()));
		
	while (enabled)
	{
//		cout << "Running the thing." << endl;
		run_java();
		update_mode();
		update_dx();
		update_dy();
		update_theta();
		if (!has_updated)
		{
			update_packet(packet_out);
			has_updated = true;
		}
	}
}

JNIEnv* com_module::create_vm(JavaVM ** jvm)
{
	printf("CREATE_VM: Entering\n");
	JNIEnv* en;
	JavaVMInitArgs vm_args;
	JavaVMOption options;
	
	printf("CREATE_VM: Java objects declared\n");

	options.optionString = "-Djava.class.path=/home/gavaga/raspberrypi/tdp.team1504.vision2014.networktables:/home/gavaga/sunspotfrcsdk/desktop-lib/networktables-desktop.jar";
	printf("Setting Options\n");
	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 1;
	vm_args.options = &options;
	vm_args.ignoreUnrecognized = 0;

	printf("Creating JVM\n");
	jint ret = JNI_CreateJavaVM(jvm, (void**)&en, &vm_args);
	if (ret < 0)
	{
		printf("\nUnable to Launch JVM\n");
	}	
	return en;
}

com_module com_module::get_instance(const string& i)
{
	ip = i;
	JavaVM * jvm;
	JNIEnv * en = create_vm(&jvm);

	com_module coms(jvm);
	coms.enable();
	coms.reset();

	return coms;
}

