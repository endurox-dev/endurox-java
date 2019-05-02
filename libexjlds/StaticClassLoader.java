package org.endurox.loader;

import java.net.URL;
import java.net.URLClassLoader;
import java.net.URLStreamHandlerFactory;

/**
 * This is extended class loader which performs following actions:
 * 1. Tries to get get class bytes from embedded resources
 * 2. Fallback to system class loader / URL Loader. Which includes
 *  resources such as -cp or -classpath on CLI and data from CLASSPATH variable.
 */
public class StaticClassLoader extends URLClassLoader {

    public StaticClassLoader(URL[] urls, ClassLoader cl) {
        super(urls, cl);
    }

    public StaticClassLoader(URL[] urls) {
        super(urls);
    }

    public StaticClassLoader(URL[] urls, ClassLoader cl, URLStreamHandlerFactory urlshf) {
        super(urls, cl, urlshf);
    }

    /**
     * We need custom loader URL here:
     * https://examples.javacodegeeks.com/core-java/net/urlclassloader/java-net-urlclassloader-example/
     * Create instance by passing list of files from -cp or -classpath.
     * Also this shall expand the URL from CLASSPATH env variable
     */
    @Override
    public Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
    
	//System.out.println("Loading class: "+name);
    
        Class<?> clazz = findLoadedClass(name);
        if (clazz == null) {
            try {
                byte [] b = getResourceBytes(name);
                
                clazz = defineClass(name, b, 0, b.length);
                if (resolve) {
                    resolveClass(clazz);
                }
            } catch (Exception e) {
                clazz = super.loadClass(name, resolve);
            }
        }
        return clazz;
    }

    @Override
    public URL getResource(String name) {
        return null;
    }
    
    /* Get class data from C side */
    public native byte[] getResourceBytes(String name);
    
}
