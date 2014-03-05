#include <jni.h>
#include <stdlib.h>
#include <stdio.h>

JNIEnv* create_vm(JavaVM ** jvm)
{
	printf("CREATE_VM: Entering\n");
	JNIEnv* env;
	JavaVMInitArgs vm_args;
	JavaVMOption options;
	
	printf("CREATE_VM: Java objects declared\n");

	options.optionString = "-Djava.class.path=/home/gavaga/raspberrypi/tdp.team1504.vision2014.networktables";
	printf("Setting Options\n");
	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 1;
	vm_args.options = &options;
	vm_args.ignoreUnrecognized = 0;

	printf("Creating JVM\n");
	jint ret = JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
	if (ret < 0)
	{
		printf("\nUnable to Launch JVM\n");
	}	
	return env;
}

int main(int argc, char** argv)
{
	printf("MAIN: Entering\n");
	JavaVM * jvm;
	printf("MAIN: declared jvm object\n");
	JNIEnv * env = create_vm(&jvm);
	printf("MAIN: created jvm object\n");
	jclass cls = (*env)->FindClass(env, "NetworkTablesModule");
	if (cls == 0)
	{
		fprintf(stderr, "Could not find class: NetworkTablesModule\n");
		exit(1);
	}

	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "test_print", "()V");
	if (mid == 0)
	{
		fprintf(stderr, "Could not find method: test_print\n");
		exit(1);
	}
	(*env)->CallStaticVoidMethod(env, cls, mid, NULL);

	(*jvm)->DestroyJavaVM(jvm);
}
