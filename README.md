# TrackMate-filter
When using TrackMate to track RBCs in vessels, do you often experience tons of garbage tracks that cannot be removed by inbuilt filters? Here is a wonderful new filter for you!

# Brief
This is a two step filter. First use the SortingAndFiltering code to filter away tracks that are moving upwards, downwards and in other false directions. Then run the FilteringAwayRidiculousPoints code to remove the rest signal noises.

# How to use:
1. Output your detected and filtered tracks from TrackMate. It should look like the csv files in testdata/, which you can use for practice.
2. You can use the speed and time in the original file to draw a graph, which will look like shit, then you can ture to my filter.
3. Open SortingAndFilteringForDA.c (I use the filter to process RBC data in zebrafish, DA and CV are vessel names of zebrafish.). The script allows you to process 6 files at a time, if you would like it different, just change the regmax in line 17. (If you have less than 6 files, remember to comment away the lines you don't use in line 52-57. Similarly, if you have more, just add the lines.) Use command or whatever method you like to get the line number of each file you have, and put them in line 10-15.
4. Compile and run! (The script allows you to filter the tracks by directionality, it keeps the tracks that go along the DA or CV and filters away those going upwards, downwards and backwards. For specific mechanisms, check the comments in the script.)
5. Open FilteringAwayRidiculousPointsForDA.c. The script allows you to process 6 files at a time, if you would like it different, just change the regmax in line 17. (If you have less than 6 files, remember to comment away the lines you don't use in line 52-57. Similarly, if you have more, just add the lines.) Use command or whatever method you like to get the line number of each file you have, and put them in line 10-15. Line 18 is for heartbeat cycle. You can get an approximate number and put it here. It doesn't have to be very accurate.
6. Again, compile and run! (The script allows you to filter the tracks by histogram. This is based on the assumption that RBCs don't speed up or slow down individually. The script deletes the lowest 10% points and highest 26% points in each time window, which is T/9. You can ajust these parameters in line 97, 157 and 158.)
7. Finally, you will get the result files which are named like DA1_final_result.csv. Now you can draw the graph again, which will look a lot nicer. You can also compare it with your manual tracking results to see if you are happy with it.
