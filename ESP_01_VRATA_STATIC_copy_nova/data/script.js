// Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyClmdEJlStE31NA_fFTqmCjZOvFhL3Qlu0",
    authDomain: "garaza-3e795.firebaseapp.com",
    projectId: "garaza-3e795",
    storageBucket: "garaza-3e795.firebasestorage.app",
    messagingSenderId: "1052443551442",
    appId: "1:1052443551442:web:355879d38230f035146f0b",
  };
  
  // Initialize Firebase
  firebase.initializeApp(firebaseConfig);
  const auth = firebase.auth();
  const provider = new firebase.auth.GoogleAuthProvider();
  
  // List of allowed (whitelisted) emails
  const allowedEmails = [
    "muhamed1911.mm@gmail.com"  // Replace with actual allowed emails
  ];
  
  document.addEventListener("DOMContentLoaded", () => {
    const googleSignInBtn = document.getElementById("googleSignInBtn");
    const openCloseBtn = document.getElementById("openCloseBtn");
    const garageSection = document.getElementById("garage-section");
    const doorSections = document.querySelectorAll('.door-section');
  
    let userEmail = "";
    let doorOpen = false;
  
    // Google Sign-In
    googleSignInBtn.addEventListener("click", () => {
      firebase.auth().signInWithPopup(provider)
        .then((result) => {
          const user = result.user;
          userEmail = user.email;
  
          // Check if the user's email is in the whitelist
          if (allowedEmails.includes(userEmail)) {
            document.getElementById("auth-section").style.display = "none";
            garageSection.style.display = "block";
            alert(`Signed in as: ${userEmail}`);
          } else {
            // Sign out the user if email is not allowed
            firebase.auth().signOut().then(() => {
              alert("You are not authorized to access this app.");
            }).catch((error) => {
              alert("Error signing out: " + error.message);
            });
          }
        })
        .catch((error) => {
          alert("Google Sign-In failed: " + error.message);
        });
    });
  
    // Open/Close Gate
    openCloseBtn.addEventListener("click", () => {
      // Toggle the animation for the gate opening/closing
      if (doorOpen) {
        doorSections.forEach((section, index) => {
          section.style.height = '25%';
        });
        doorOpen = false;
      } else {
        doorSections.forEach((section, index) => {
          section.style.height = '0%';
        });
        doorOpen = true;
      }
    });
  });  