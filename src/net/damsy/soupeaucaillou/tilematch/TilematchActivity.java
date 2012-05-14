package net.damsy.soupeaucaillou.tilematch;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.openfeint.api.OpenFeint;
import com.openfeint.api.OpenFeintDelegate;
import com.openfeint.api.OpenFeintSettings;
import com.openfeint.api.resource.Achievement;
import com.openfeint.api.resource.Leaderboard;

import android.app.Activity;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager.LayoutParams;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;

public class TilematchActivity extends Activity {
	static final String gameName = "Feuilles";
	static final String gameID = "469623";
	static final String gameKey = "yWoOQ6qClGfYZY3ggwqobQ";
	static final String gameSecret = "iR3MB4J7ZuBr90CGrPElAC4lgD0nLkWNPdXxQ2e7Ck"; 
	
	public static List<Achievement> achievements = null;
	public static List<Leaderboard> leaderboards = null;
	  
	static final String TILEMATCH_BUNDLE_KEY = "plop";
	static public long game = 0 	;
	static public Object mutex;
	static public byte[] savedState;
	static public int openGLESVersion = 2;
	byte[] renderingSystemState;
	static public SoundPool soundPool;
	static public boolean isRunning;
	static public TilematchStorage.OptionsOpenHelper optionsOpenHelper;
	static public TilematchStorage.ScoreOpenHelper scoreOpenHelper;
	static public View playerNameInputView;
	static public EditText nameEdit;
	static public String playerName;
	static public boolean nameReady;
	
	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Map<String, Object> options = new HashMap<String, Object>();
        options.put(OpenFeintSettings.SettingCloudStorageCompressionStrategy, OpenFeintSettings.CloudStorageCompressionStrategyDefault);
        // use the below line to set orientation
        // options.put(OpenFeintSettings.RequestedOrientation, ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        OpenFeintSettings settings = new OpenFeintSettings(
        	    gameName, gameKey, gameSecret, gameID, options); 
        OpenFeint.initialize(this, settings, new OpenFeintDelegate() { });
        
        Achievement.list(new Achievement.ListCB() {
			@Override public void onSuccess(List<Achievement> _achievements) {
				achievements = _achievements;
			}
		});
        
        Leaderboard.list(new Leaderboard.ListCB() {
			@Override public void onSuccess(List<Leaderboard> _leaderboards) {
				leaderboards = _leaderboards;
			}
		});
        
        mutex = new Object();
 
        getWindow().setFlags(LayoutParams.FLAG_FULLSCREEN,
    			LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.main);
       
        RelativeLayout rl = (RelativeLayout) findViewById(R.id.parent_frame);
        
        Renderer r = new TilematchRenderer(getAssets());
        assert(r != null);
        try {
        	if (true) throw new RuntimeException();
        	mGLView = new GL2JNIView(this, r, null);
        	TilematchActivity.openGLESVersion = 2;
        } catch (Exception exc) {
        	Log.e("tilematchJava", "Failed to create OpenGL ES2 context, try ES1");
        	mGLView = new GLSurfaceView(this);
        	mGLView.setRenderer(r);
        	TilematchActivity.openGLESVersion = 1;
        }
         
        mGLView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        rl.addView(mGLView);
        playerNameInputView = findViewById(R.id.enter_name);
        nameEdit = (EditText) findViewById(R.id.player_name_input);
        rl.bringChildToFront(playerNameInputView);
        playerNameInputView.setVisibility(View.GONE);
        
        Button b = (Button) findViewById(R.id.name_save);
        b.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				playerName = nameEdit.getText().toString();
				Log.i("tilematchJ", "Player name: '" + playerName + "'");
				if (playerName != null && playerName.length() > 0) {
					playerNameInputView.setVisibility(View.GONE);
					TilematchActivity.nameReady = true;
				}
			}
		}); 
        
        TilematchActivity.scoreOpenHelper = new TilematchStorage.ScoreOpenHelper(this);
        TilematchActivity.optionsOpenHelper = new TilematchStorage.OptionsOpenHelper(this);
        TilematchActivity.soundPool = new SoundPool(8, AudioManager.STREAM_MUSIC, 0);
        
        if (savedInstanceState != null) {
	        TilematchActivity.savedState = savedInstanceState.getByteArray(TILEMATCH_BUNDLE_KEY);
	        if (TilematchActivity.savedState != null) {
	        	Log.i("tilematchJava", "State restored from app bundle");
	        } else {
	        	Log.i("tilematchJava", "WTF?");
	        }
        } else {
        	Log.i("tilematchJava", "savedInstanceState is null");
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
        TilematchJNILib.pause(TilematchActivity.game);
        isRunning = false;
    }

    @Override
    protected void onResume() {
        super.onResume();
        isRunning = true;
        mGLView.onResume();
    }
    
    @Override
    protected void onSaveInstanceState(Bundle outState) {
    	/* save current state; we'll be used only if app get killed */
    	synchronized (TilematchActivity.mutex) {
	    	Log.i("tilematchJava", "Save state!");
	    	byte[] savedState = TilematchJNILib.serialiazeState(TilematchActivity.game);
	    	if (savedState != null) {
	    		outState.putByteArray(TILEMATCH_BUNDLE_KEY, savedState);
	    	}
	    	Log.i("tilematchJava", "State saved");
    	}
    	super.onSaveInstanceState(outState);
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
    	int action = event.getAction();
    	
    	if (action == MotionEvent.ACTION_DOWN || action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_MOVE) {
    		TilematchJNILib.handleInputEvent(game, event.getAction(), event.getX(), event.getY());
    		return true;
    	}
    	
    	return super.onTouchEvent(event);
    }
    
    @Override
    public void onBackPressed() {
    	
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
    	if (keyCode == KeyEvent.KEYCODE_MENU) {
    		TilematchJNILib.pause(game);
    	}
    	return super.onKeyUp(keyCode, event);
    }
    
    public static GLSurfaceView mGLView;

    static {
        System.loadLibrary("tilematch");
    }
}
