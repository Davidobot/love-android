/**
 * Created by Martin Braun (Marty) for love2d
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

package org.love2d.android;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.common.api.Status;
import com.google.android.gms.drive.Drive;
import com.google.android.gms.games.AchievementsClient;
import com.google.android.gms.games.AnnotatedData;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.LeaderboardsClient;
import com.google.android.gms.games.PlayersClient;
import com.google.android.gms.games.SnapshotsClient;
import com.google.android.gms.games.achievement.Achievement;
import com.google.android.gms.games.achievement.AchievementBuffer;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;

import static com.google.android.gms.common.GooglePlayServicesUtil.isGooglePlayServicesAvailable;


// Pay


// Firebase


public class RichGameActivity extends GameActivity {

	private static final String TAG = "RichGameActivity";

	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		/*if (!isTaskRoot()
				&& getIntent().hasCategory(Intent.CATEGORY_LAUNCHER)
				&& getIntent().getAction() != null
				&& getIntent().getAction().equals(Intent.ACTION_MAIN)) {
			finish();
			return;
		}*/
		super.onCreate(savedInstanceState);
	}

	@Override
	protected void onStart() {
		super.onStart();

	}
	
	@Override
    protected void onDestroy() {
		super.onDestroy();
		System.exit(0); // rough, make sure all threads getting killed (hack)
    }

    @Override
    protected void onPause() {
		super.onPause();

    }

    @Override
    public void onResume() {
		super.onResume();

    }

	@Override
	protected void onActivityResult(final int requestCode, final int resultCode, final Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		switch(requestCode) {
			case MOBSVC_SIGN_IN_REQ:
				mobSvcSignInActivityCallback(resultCode, data);
				break;
		}
	}

	@Override
	protected void onNewIntent (Intent intent) {
		/* This function causes trouble, so we shallow it
		The function in the original activity was finishing the existent intent and creating a new one, causing the game to restart!
		 */
	};

	//region MobSvc

	private static final int MOBSVC_SIGN_IN_REQ = 0x10;

	private boolean mobSvcAvailable = false;
	private GoogleSignInClient mobSvcClient;
	private GoogleSignInAccount mobSvcAccount;
	private PlayersClient mobSvcPlayerClient;
	private String mobSvcPlayerId;
	private AchievementsClient mobSvcAchievementsClient;

	private void alertMissingGms() {
		AlertDialog alertDialog = new AlertDialog.Builder(this).create();
		alertDialog.setTitle("Google Play Games Unavailable");
		alertDialog.setMessage("Player is not signed in");
		alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "OK",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
					}
				});
		alertDialog.show();
	}

	public void mobSvcInit(final boolean allowGDrive) {
		// Determine if Google Play Games Services is installed on the device to make this optional. Will treat disabled services as installed.
		GoogleApiAvailability gaa = GoogleApiAvailability.getInstance();
		//boolean isKindle = (Build.MANUFACTURER.equals("Amazon") && Build.MODEL.equals("Kindle Fire")) || Build.MODEL.startsWith("KF");

		if (gaa.isGooglePlayServicesAvailable(this) == ConnectionResult.SUCCESS) {
			mobSvcAvailable = true;

			GoogleSignInOptions.Builder gsoBuilder = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN);
			if(allowGDrive) {
				gsoBuilder = gsoBuilder.requestScopes(Drive.SCOPE_APPFOLDER);
			}
			GoogleSignInOptions gso = gsoBuilder.build();
			mobSvcClient = GoogleSignIn.getClient(this, gso);
			Log.d(TAG,"Play Games Services initialised: " + new ConnectionResult(gaa.isGooglePlayServicesAvailable(this)).toString());
		} else {
			Log.d(TAG, "Play Game Services unavailable: " + new ConnectionResult(gaa.isGooglePlayServicesAvailable(this)).toString());
		}
	}

	boolean mobSvcSignInWait = false;
	public String mobSvcSignInAwait() {
		Log.d(TAG, "Signing in");
		if(mobSvcAvailable && !mobSvcSignInWait) {
			mobSvcPlayerId = null;
			mobSvcAccount = GoogleSignIn.getLastSignedInAccount(this);
			if (mobSvcAccount == null && mobSvcClient != null) {
				mobSvcSignInWait = true;
				// play around with this; maybe silent sign-in when the last signed-in account is OK
				Intent intent = mobSvcClient.getSignInIntent();
				startActivityForResult(intent, MOBSVC_SIGN_IN_REQ);
				/*mobSvcClient.silentSignIn().addOnCompleteListener(this, new OnCompleteListener<GoogleSignInAccount>() {
					@Override
					public void onComplete(@NonNull Task<GoogleSignInAccount> task) {
						if (task.isSuccessful()) {
							// The signed in account is stored in the task's result.
							mobSvcAccount = task.getResult();
							mobSvcSignInWait = false;
						} else {
							runOnUiThread(new Runnable() {
								@Override
								public void run() {
									// Player will need to sign-in explicitly using via UI
									Intent intent = mobSvcClient.getSignInIntent();
									startActivityForResult(intent, MOBSVC_SIGN_IN_REQ);
								}
							});
						}
					}
				});*/
			} // meanwhile:
			while (mobSvcSignInWait) SystemClock.sleep(100); // await, runs forever if the player won't login (just to stay consistent, because iOS provides no deny/fail callback)
			if(mobSvcAccount != null) {
				mobSvcSignInWait = true;
				mobSvcPlayerClient = Games.getPlayersClient(this, mobSvcAccount);
				mobSvcPlayerClient.getCurrentPlayerId().addOnCompleteListener(this, new OnCompleteListener<String>() {
					@Override
					public void onComplete(@NonNull Task<String> task) {
						if (task.isSuccessful()) {
							mobSvcPlayerId = task.getResult();
							Log.d(TAG, "Successfully signed into Play Games Services.");
						} else {
							mobSvcPlayerId = null;
							Log.d(TAG, "Successfully signed into Play Games Services, but failed receiving the player ID.");
						}
						mobSvcSignInWait = false;
					}
				}); // meanwhile:
				mobSvcAchievementsClient = Games.getAchievementsClient(this, mobSvcAccount);
				while (mobSvcSignInWait) SystemClock.sleep(100); // await getting player ID
			}
			return mobSvcPlayerId;
		}
		return null;
	}
	private void mobSvcSignInActivityCallback(final int resultCode, final Intent data) {
		Task<GoogleSignInAccount> task = GoogleSignIn.getSignedInAccountFromIntent(data);

		try {
			mobSvcAccount = task.getResult(ApiException.class);
		} catch (ApiException e) {
			// The ApiException status code indicates the detailed failure reason.
			// Please refer to the GoogleSignInStatusCodes class reference for more information.
			Log.w(TAG, "signInResult:failed code=" + e.getStatusCode());
		}

		mobSvcSignInWait = false;
	}

	public boolean mobSvcIsSignedIn() {
		return GoogleSignIn.getLastSignedInAccount(this) != null;
	}

	public void mobSvcShowAchievements() {
		if(mobSvcAchievementsClient != null) {
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					mobSvcAchievementsClient.getAchievementsIntent().addOnCompleteListener(new OnCompleteListener<Intent>() {
						@Override
						public void onComplete(@NonNull Task<Intent> task) {
							if(task.isSuccessful()) {
								Intent intent = task.getResult();
								startActivityForResult(intent, 0x00);
							}
							else {
								Exception ex = task.getException();
								Log.d(TAG, "Failed to load achievements: " + (ex != null ? ex.getMessage() : "UNKNOWN"));
							}
						}
					});
				}
			});
		}
		else {
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					alertMissingGms();
				}
			});
		}
	}

	boolean mobSvcIncrementAchievementProgressWait = false;
	boolean mobSvcIncrementAchievementProgressResult = false;
	boolean mobSvcIncrementAchievementNowUnlocked = false;
	public boolean[] mobSvcIncrementAchievementProgressAwait(final String achievementsId, int steps, int maxSteps) {
		if(mobSvcAchievementsClient != null && steps > 0) {
			mobSvcIncrementAchievementProgressResult = false;
			mobSvcIncrementAchievementNowUnlocked = false;
			if(mobSvcIncrementAchievementProgressWait) {
				mobSvcIncrementAchievementProgressWait = false;
				SystemClock.sleep(2000); // give other threads of the same operation time to abort, this will result in false operations that can still succeed in background, later. To avoid problems, please nest same operations in callbacks
			}
			mobSvcIncrementAchievementProgressWait = true;
			if(maxSteps < 2) { // On maxSteps 1 we know this is not an incremental achievement, so we simply solve it when any step is given
				mobSvcAchievementsClient.unlockImmediate(achievementsId).addOnCompleteListener(new OnCompleteListener<Void>() {
					@Override
					public void onComplete(@NonNull Task<Void> task) {
						if(task.isSuccessful()) {
							mobSvcIncrementAchievementProgressResult = true;
							mobSvcIncrementAchievementNowUnlocked = true;
						} else {
							mobSvcIncrementAchievementProgressResult = false;
							mobSvcIncrementAchievementNowUnlocked = false;
							Exception ex = task.getException();
							Log.d(TAG, "Failed to unlock achievement: " + (ex != null ? ex.getMessage() : "UNKNOWN"));
						}
						mobSvcIncrementAchievementProgressWait = false;
					}
				});
			} else { // on maxStep 2 or more we know this is an incremental achievement, so we simply pass the step value through
				mobSvcAchievementsClient.incrementImmediate(achievementsId, steps).addOnCompleteListener(new OnCompleteListener<Boolean>() {
					@Override
					public void onComplete(@NonNull Task<Boolean> task) {
						if(task.isSuccessful()) {
							mobSvcIncrementAchievementProgressResult = true;
							mobSvcIncrementAchievementNowUnlocked = task.getResult(); // BUG: This is always false, even when the achievement is unlocked now, so we have to do:
							mobSvcAchievementsClient.load(false).addOnCompleteListener(new OnCompleteListener<AnnotatedData<AchievementBuffer>>() {
								@Override
								public void onComplete(@NonNull Task<AnnotatedData<AchievementBuffer>> task) {
									if(task.isSuccessful()) {
										AnnotatedData<AchievementBuffer> achBufferData = task.getResult();
										AchievementBuffer achBuffer = achBufferData.get();
										if(achBuffer != null) {
											for (Achievement ach : achBuffer) {
												if (ach.getAchievementId().equals(achievementsId)) {
													mobSvcIncrementAchievementNowUnlocked = ach.getState() == Achievement.STATE_UNLOCKED;
													break;
												}
											}
										}
										achBuffer.release();
									} else {
										Exception ex = task.getException();
										Log.d(TAG, "Failed to determine achievement status: " + (ex != null ? ex.getMessage() : "UNKNOWN"));
									}
									mobSvcIncrementAchievementProgressWait = false;
								}
							});
						} else {
							mobSvcIncrementAchievementProgressResult = false;
							mobSvcIncrementAchievementNowUnlocked = false;
							mobSvcIncrementAchievementProgressWait = false;
							Exception ex = task.getException();
							Log.d(TAG, "Failed to increment achievement progress: " + (ex != null ? ex.getMessage() : "UNKNOWN"));
						}
					}
				});
			} // meanwhile:
			while (mobSvcIncrementAchievementProgressWait) SystemClock.sleep(100); // await result.
			return new boolean[] { mobSvcIncrementAchievementProgressResult, mobSvcIncrementAchievementNowUnlocked };
		}
		return new boolean[] { false, false };
	}

	//endregion
}