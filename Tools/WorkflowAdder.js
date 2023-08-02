// !!! Built between lecs on the last day to add courses !!!
// 1. Login to Workflow
// 2. Go to the 'Add/Drop Courses' tab
// 3. Select course code & category
// 4. Open the browser's Javascript console (Ctrl/Cmd + Shift + J or F12)
// 5. Copy & paste the code below into the console, then press Enter

async function submit() {
    do {
        document.getElementById("ctl00_MainContent_btnNonCurAdd").click();
        console.log(new Date().toLocaleTimeString() + " Clicked!");
        await new Promise(resolve => setTimeout(resolve, 5000)); // 5 seconds
    } while (document.getElementById("ctl00_MainContent_lblErrorMsg").textContent === "There is no vacancy for this course");

    document.getElementById("ctl00_MainContent_gvBasketCourses_ctl02_chkCell_CC").click();
    document.getElementById("ctl00_MainContent_btnSubmit").click();
    console.log(new Date().toLocaleTimeString() + " Submitted!");
}

submit();
