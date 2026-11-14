# The 1 Dec run's video analysis (as run, 14 Nov)

Working scripts, kept so the numbers in docs/HISTORY.md (33rd and 34th entries) can be re-derived. They run in a folder
holding the intermediate files, made with ffmpeg from the owner's two videos (not in the repo):
- `row300.raw`: one grey row per phone frame (`crop=1700:1:20:300`); `phone_sx.npy`: the shift between consecutive
  rows (FFT correlation, +-40 px), made as in HISTORY;
- `macrow.raw` (`format=gray,crop=2200:1:400:600`, `-fps_mode passthrough`) and `mac_fts.csv` (ffprobe frame
  pts_time): the screen recording, every frame with its time;
- `cam_T.npy`, `cam_Y.npy`: the camera's yaw from the frame log (phone_align.py writes them).
compare_starts.py and refresh_phase.py read only the results zip (PresentMon and the frame logs).
