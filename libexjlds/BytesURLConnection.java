package org.endurox.loader;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

class BytesURLConnection extends URLConnection {
    
    public BytesURLConnection(URL url) {
        super(url);
    }

    @Override
    public void connect() throws IOException {
    }

    @Override
    public InputStream getInputStream() throws IOException {
        return new ByteArrayInputStream(getResourceBytes(this.getURL().getPath().substring(1)));
    }
    
    /* Get class data from C side */
    public native byte[] getResourceBytes(String name);
    
    /* read from resource list */
    public native byte[] getClassBytes(String name);
}

