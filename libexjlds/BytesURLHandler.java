package org.endurox.loader;

import java.io.IOException;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;

/**
 * This is extended class loader which performs following actions:
 * 1. Tries to get get class bytes from embedded resources
 * 2. Fallback to system class loader / URL Loader. Which includes
 *  resources such as -cp or -classpath on CLI and data from CLASSPATH variable.
 */
public class BytesURLHandler extends URLStreamHandler {
    
     @Override
    protected URLConnection openConnection(URL u) throws IOException {
        return new BytesURLConnection(u);
    }       
}