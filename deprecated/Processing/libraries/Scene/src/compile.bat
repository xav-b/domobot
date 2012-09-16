javac -O -source 1.5 -target 1.5 -cp libTUIO.jar:core.jar scene\tuio\*.java
jar cfm ..\library\SceneTuio.jar manifest.inc scene\tuio\SceneTuioBlob.class scene\tuio\SceneTuioListener.class scene\tuio\SceneTuioClient.class scene\tuio\SceneBlob.class
jar cfm ..\library\Scene.jar manifest.inc scene\tuio\Scene.class
del scene\tuio\*.class