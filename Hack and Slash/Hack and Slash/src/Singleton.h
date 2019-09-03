#pragma once
/*
Don't inherit this class to create a singleton, follow this structure and replicate it in another class to create it using the singleton pattern
(singleton pattern: only having 1 instance/object of a defined class). This implementation is thread safe and solves issues, but WARNING!
The singleton pattern should be avoided as it difficults unit testing and because they are generally used as a global instance, why is that so bad?
Because you hide the dependencies of your application in your code, instead of exposing them through the interfaces. Making something global to avoid
passing it around is a code smell. Use singletons if  you have a resource that can only have a single instance and you need to manage that single one.
https://stackoverflow.com/questions/137975/what-is-so-bad-about-singletons
https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template
*/

// Singleton Class
class Singleton {
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton &&) = delete;
	Singleton& operator=(Singleton &&) = delete;

	static Singleton& instance() {
		static Singleton instance;
		return instance;
	}

private:
	Singleton() {} // private constructor so that no objects can be created (disallow instantiation outside of the class)
};

// Singleton Template Class
template <class T>
class SingletonTemplate {
public:
	SingletonTemplate(const T&) = delete;
	SingletonTemplate& operator=(const T&) = delete;
	SingletonTemplate(T &&) = delete;
	SingletonTemplate& operator=(T &&) = delete;

	static T& instance() {
		static T instance;
		return instance;
	}

private:
	SingletonTemplate() {} // private constructor so that no objects can be created (disallow instantiation outside of the class)
};