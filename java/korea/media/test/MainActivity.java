package korea.media.test;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {

    private long PressTime = 0;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // create OpenGL ES surface view
        GLSurfaceView glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(2);
        glSurfaceView.setRenderer(new GLSurfaceRenderer());

        setContentView(glSurfaceView);
    }

    @Override
    public boolean onTouchEvent (MotionEvent event)
    {
        float x = event.getX();
        float y = event.getY();
        int count = event.getPointerCount();
        switch(event.getActionMasked())
        {
            case MotionEvent.ACTION_DOWN:
                if(count ==2 ) mouseDownEvents(x,y, true);
                else if(count == 1) mouseDownEvents(x,y, true);
                break;

            case MotionEvent.ACTION_MOVE:
                if(count ==2 ) mouseMoveEvents(x,y, true);
                else if(count == 1) mouseMoveEvents(x,y, true);
                break;

            case MotionEvent.ACTION_UP:
                break;
        }
        return true;
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void surfaceCreated(AssetManager assetManager);
    public native void surfaceChanged(int width, int height);
    public native void drawFrame(float deltaTime);

    public native void mouseDownEvents(float x, float y, boolean doubleTouch);
    public native void mouseMoveEvents(float x, float y, boolean doubleTouch);
    private class GLSurfaceRenderer implements GLSurfaceView.Renderer {

        // last time attribute for calculating delta time
        private long lastTime = System.currentTimeMillis();

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {

            // call native method
            surfaceCreated(getAssets());
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

            // call native method
            surfaceChanged(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {

            // calculate delta time
            long currentTime = System.currentTimeMillis();
            float deltaTime = (float) (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // call native method
            drawFrame(deltaTime);
        }
    }




}
