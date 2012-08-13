CLASSPATH=.:gson-2.2.2.jar
CLASSFILES=Server.class
		  

all: $(CLASSFILES)

%.class: %.java
	javac -classpath $(CLASSPATH) $<

exec:
	java -classpath $(CLASSPATH) Server

clean:
	rm -f *.class
