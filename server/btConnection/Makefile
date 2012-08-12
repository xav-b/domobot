CLASSPATH=.:bluecove-gpl-2.1.0.jar:bluecove-2.1.0.jar
CLASSFILES=BluetoothPort.class BluetoothThread.class BluetoothTargetHandler.class
		  

all: $(CLASSFILES)

%.class: %.java
	javac -classpath $(CLASSPATH) $<

exec:
	java -classpath $(CLASSPATH) BluetoothPort

clean:
	rm -f *.class
