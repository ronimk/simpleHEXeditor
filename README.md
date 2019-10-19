# simpleHEXeditor
A small project for the fall break 2019
<br>
<br>
Tested with MCUXpresso and CodeBlocks
<br>
<br>
List of available commands:
<br>
?        - to print help [INCOMPLETE]<br>
show     - to show a block of bytes of an opened file in HEX format [COMPLETE]<br>
del      - to delete a block of bytes from the file [COMPLETE]<br>
add      - to add bytes into the open file [COMPLETE]<br>
mod      - to modify a block of bytes in the file [COMPLETE]<br>
filesize - to print the size of the open file [COMPLETE]<br>
sav      - to save any changes made to the file [COMPLETE]<br>
ld       - to open a desired file [COMPLETE]<br>
apply    - to apply a previous patch to the file [COMPLETE]<br>
quit     - quits the program [COMPLETE]
<br>
<br>
Deadline 20.10.
<br>
Mostly tested. A bit more testing required still
<br>
<br>
TODO:
<br>
clean up eval_apply and reverse_command<br>
<br>
?    - refine the help info for each command. <br>
upload the design documents<br>
refactoring and finetuning [MOSTLY DONE]<br>
<br>
Fixed problems:<br>
CRITICAL: due to the program using unsigned ints, a loop in eval_add broke if given 0 as an argument. [FIXED]
CRITICAL: filesize (and max_filesize) was not handled correctly causing memory leaks, wrong allocations/deallocations,
and data being pushed outside file_data buffer. [FIXED]<br>
CRITICAL: fix problems concerning signed/unsigned chars when modifying or examining the file_data buffer. [FIXED]<br>
<br>
<br>
Known issues:<br>
The maximum sie of a file is constrained by the size of unsigned int in the system.<br>
<br>
The project was originally designed to run on an LPCXpresso microcontroller, and having very limited RAM size, it was necessary to use as simple and light data structure for handling the loading of the file into memory. Thus a simple unsigned char array was used for that.<br>
<br>
This meant that a simple variable like "int max_filesize" had to be used for marking the maximum size of the file, and also integer variables would be used for indexing the file_data buffer.<br>
<br>
Using unsigned int instead of int as the basic datatype would, in theory, double the maximum size of the files the program could handle, thus making the project more portable to bigger systems, too. In hindsight, following the same logic a long or even a long long would have been an even better choice...<br>
<br>
Though, it is not difficult at all to change the system to have a more advanced data structure for handling the file_data, should one choose to, and hopefully that will be included in a future revision. However, a more advance ds for file_data handling would mean more space required for each byte in the opened file and less space available for the LPCXpresso chip!<br>
<br>
In hindsight, choosing to work with unsigned ints was a stupid decision, but it seemed "clever" enough when the project started.<br>
<br>
In any case, a MAX_FILESIZE-limit should be introduced and checked whenever needed in order to prevent the system breaking when the size of the file being modified approaches the maximum limits the system can handle. Otherwise a user might (intentionally or not) try to add, for example, past the maximum indexable array element...<br>
<br>
<br>
Applying a patch, not to speak of rverting back to an older version of an edited file (by reverse apply) is handled very slowly intentionally - as strange as that may sound. The LPCXpresso chip has so little heap space that I didn't want to waste any more of it than necessary. Thus a patchfile is being read and processed on the fly, without loading any more of it than a single line at a time into the program memory.<br>
<br>
<br>
Last but not least: everything else turned out pretty neat, but eval_apply (and reverse_command) is currently a bit of a mess. It's been rigorously tested, so it should work, but the code itself could be much cleaner.<br>
<br>
<br>
Future revisions:<br>
* Enable a mode where the user can choose if a file is to be logged for patching (easy to introduce now that the foundations for this are already coded in).<br>
* Make the file handling more flexible for very big files. 
