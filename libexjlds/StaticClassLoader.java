package org.endurox.loader;

import java.net.URL;


public class StaticClassLoader extends ClassLoader {

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
