ΚΑΛΑΜΠΟΚΗΣ ΕΥΑΓΓΕΛΟΣ
1115202100045


ΣΗΜΑΝΤΙΚΕΣ ΠΑΡΑΤΗΡΗΣΕΙΣ:
/////////////////////////////////////////////////////////////////////////////////////////////////////////

ΑΡΧΕΙΑ:
queue.c: υλοποιεί τις συναρτήσεις τις ουράς που χρειάστηκα 

queue.h: Περιέχει τις βασικές συναρτήσεις που χρησιμοποιούνται για την ουρά και κάποιες σημαντικές βοηθητικές μεταβλητές και συναρτήσεις του issuejob

issueJob: Αποτελεί μια συνάρτηση που καλείται όταν ληφθεί από τον σέρβερ η εντολή issueJob ή από την updated_Concurrency και την job_handler όταν αφαιρεί ένα job 

Makefile: Κάνοντας make γίνονται compiled όλα τα απαραίτητα αρχεία και σβήνεται ότι χρειάζεται κάνοντας make clean

multijob.sh: bash script που τρέχει εντολές από αρχείο που δίνεται από το terminal

jobCommander, jobServer: Αποτελούν τα βασικά αρχεία της εργασίας που ζητούνται

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ΕΚΤΕΛΕΣΗ ΕΝΤΟΛΩΝ:

./jobCommander issueJob "job"

./jobCommander setConcurrency "number"

./jobCommander poll "running | queued"

./jobCommander stop "id"

./jobCommander exit

./multijob.sh "file1.txt" "file2.txt" ......

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ΒΑΣΙΚΗ ΙΔΕΑ:

Υπάρχει το αρχείο jobCommander.C που δέχεται τις εντολές ανοίγει τα pipes για επικοινωνία και αναλογά με το αν είναι ή όχι ανοιχτό το σέρβερ ανοίγει τον κατάλληλο σεμαφώρο και τον ίδιο σέρβερ εαν χρειάζεται έχει διάρκεια ζωής μια εντολή και στέλνει σωστά τις εντολές στον σέρβερ.

Υπάρχει το jobExecutorServer.c που χειρίζεται το reading από το FIFO_FILE γράφει το pid στο ζητούμενο txt και αφού το κάνει ειδοποιεί τον commander με σεμαφώρους μετά χειρίζεται τις εντολές. Για την κάθε εντολή στέλνει και την ζητόυμενη απάντηση στον commander μέσω της send_answer(). Όταν λάβει issueJob καλεί την αντίστοιχη συνάρτηση που είναι υπέυθυνη για την εκτέλεση την προσθήκη και αφαίρεση από ουρές καθώς και την διαχείριση της αλλαγής του concurrency.

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ΠΡΟΒΛΗΜΑΤΑ:

Δυστυχώς λόγω περιορισμένου χρόνου δεν κατάφερα να λύσω το εξής θέμα στην εργασία:
Παρατήρησα ότι όταν θέσω το concurrency να είναι > 2 και ΕΦΟΣΟΝ έχουν ήδη προστεθεί αρκέτα jobs στις αντίστοιχες ουρές τότε υπάρχει μια πιθανότητα (δε συμβαίνει πάντα) η επόμενη εντολή που συνήθως είναι η poll να μην φτάσει στον server καθώς κολλάει ο commander στο named pipe που θα στείλει την εντολή. Η υπόθεση είναι πως υπάρχει κάποιο πρόβλημα στον συγχρονισμό και την χρήση των named pipes από την setConcurrency που δυστυχώς αποκαλύπτεται μερικές φορές με τον τρόπο που περιέγραψα παραπάνω. Επίσης για τον ίδιο λόγο (πρόβλημα με τον συγχρονίσμο του named pipe που στέλνει απαντήσεις στον commander) η poll δε στέλνει όλες τις τριπλέτες αλλά μόνο την πρώτη, παρόλα αυτά άφησα σε comments την ιδέα μου η οποία ήταν αρκέτα κοντά στη λύση σε περίπτωση που εκτιμηθεί. Τέλος δεν ολοκλήρωσα το δεύτερο script που σβήνει όλες τα jobs διότι δεν λειτουργούσε ολοκληρώμενα όπως ανέφερα η poll οπότε δεν μπορούσα να βρω όλα τα jobs για να τα σβήσω :( 

/////////////////////////////////////////////////////////////////////////////////////////////////////////
