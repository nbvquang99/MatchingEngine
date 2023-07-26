## Table of contents

- [I. Description](#i-description)
- [II. App Description](#ii-app-description)
  - [1. The story](#1-the-story)
  - [2. The app features](#2-the-app-features)
    - [*Account system*](#account-system)
    - [*Schedule \& meeting management*](#schedule--meeting-management)
    - [*Group management*](#group-management)
    - [*Place suggestion*](#place-suggestion)
- [III. Code Description](#iii-code-description)
  - [1. Techstack](#1-techstack)
  - [2. Class Structure](#2-class-structure)
    - [**User**](#user)
    - [**Group**](#group)
    - [**Meeting**](#meeting)
    - [**Place**](#place)
    - [**Map API**](#map-api)
  - [3. Sequence of interaction](#3-sequence-of-interaction)
- [IV. Challenges](#iv-challenges)
- [V. Acknowledge](#v-acknowledge)
---
# I. Description

1. **Group 2**: 
   - 20190710 Nguyen Ba Vinh Quang / nbvquang99@kaist.ac.kr
   - 20200510 이지윤 (Nicole Lee) / nicolelee2001@kaist.ac.kr
   - 20200773 이수연 (Suyoun Lee) / jenslee705@kaist.ac.kr
2. **GIT URL**: [https://github.com/babycroc/id311-lets-meek](https://github.com/babycroc/id311-lets-meek)
3. **Deploy Link**: [https://id311-lets-meek.vercel.app/](https://id311-lets-meek.vercel.app/)
4. **Youtube Demo Video**: [Click here](https://youtu.be/VYxQf9JLeqs)

# II. App Description
<p align="center">
  <img src="static/logo.png" alt="Logo">
</p>

## 1. The story
This app focuses on KAIST students. It helps to manage the weekly class schedule and also group meetings since every KAIST student would join multiple groups each semester.

## 2. The app features
*In this section, briefly describe application features (for more details and visualization, please refer to [Demo Video](https://youtu.be/VYxQf9JLeqs)).*
### *Account system*
- User can `create account` with email and `login` to use the function of the app.
### *Schedule & meeting management*
- User can add class schedule to the app with the location information, such as `10am-12pm, N25 building`.
- User can also create meeting in the empty timeframes which is not occured by class sessions.
- When the meeting time is passed, the app will automatically delete the meeting from user's schedule.
- Each meeting belongs to one group and it would be showed in the meeting list with the color matching with the color of the group.

### *Group management*
- `A group` include multiple users. User scan create many groups as they want to.
- Users can also join a group by the invitation code. Each group will have a fixed invitation code.
- From the group list page, user can create new meeting for a specific group.

### *Place suggestion*
- The app bases on the location that users input to schedule and location of other meetings to suggest a place for a new meeting so that the total travel time for every members in the group is minimized.
- Place also is separated into 3 types of environment sound: `loud, quite, and moderate`.

# III. Code Description
## 1. Techstack
<p align="center">
  <img src="static/stack.png" alt="Logo">
</p>

## 2. Class Structure
### **User**
User class represents for User document on firebase database. When user login, an User object is returned.
### **Group**
Group class represents for Group document on firebase database. When user create a new group, a Group object is created. When user join a group, the user id is pushed into the group object.
### **Meeting**
Meeting class represents for Meeting document on firebase database. When user create a new meeting for the current group, a Meeting object is created. Then the Meeting object will be linked with the current group and every users in that group.
### **Place**
Place class represents for Place document on firebase database. Theses places were added manually by our team in order to give the suggestion for meeting place.
### **Map API**
This is not a class, it is a collection of functions that utilize the Google Map API and Kakao map API to compute travel time, distance, get location... in order to give the suggestion places.
## 3. Sequence of interaction
<p align="center"><img src="static/Slide 16_9 - 7.png" width=100% height=100% alt></p>
<p align="center"><img src="static/Slide 16_9 - 12.png" width=100% height=100% alt></p>
<p align="center"><img src="static/Slide 16_9 - 13.png" width=100% height=100% alt></p>
<p align="center"><img src="static/Slide 16_9 - 16.png" width=100% height=100% alt></p>
<p align="center"><img src="static/Slide 16_9 - 15.png" width=100% height=100% alt></p>

# IV. Challenges
1. Learning Curve: Familiarizing ourself with SvelteKit, daisyUI, Firebase Authentication, Firestore, and the map APIs may take some time and effort.

2. Integration: Integrating multiple technologies can be challenging, especially when they have different APIs and conventions. Ensuring smooth integration between SvelteKit, daisyUI, Firebase Authentication, Firestore, and the map APIs requires careful attention to documentation and best practices. Moreover, althought the backend API is documented with JSdoc, it is still difficult to integrate between Backend and Frontend, a lot of bugs occured, and requiring multiple meetings and parallel programming session to resolve.

3. Authentication and Authorization: Implementing user authentication and authorization using Firebase Authentication is not complex, but when combined with other services like Firestore. We face challenges in setting up managing user sessions, and storing user data into database.

4. Real-time Updates: Firestore offers real-time updates, allowing us to listen for changes in the database. However, incorporating real-time updates into our SvelteKit application require understanding the structure of the backend code.

5. Map API Integration: Integrating the Google Map API or Kakao Map API does pose challenges, such as obtaining API keys, setting up proper authentication and authorization, and ensuring smooth interactions between the maps and your application's data. The API quotas is also a problem.

6. Debugging and Troubleshooting: As with any software development project, debugging and troubleshooting is very time-consuming and challenging. Dealing with issues related to data flow, API integrations, or promises and async functions require careful investigation and problem-solving skills.

# V. Acknowledge
- Reference: 
  - [Google Map API document](https://developers.google.com/maps/documentation/javascript/markers)
  - [Kakao Map Rest API document](https://developers.kakaomobility.com/docs/navi-api/origins/)
  - [Firebase document](https://firebase.google.com/docs/firestore/query-data/get-data#get_all_documents_in_a_collection)
  - [DaisyUI document](https://daisyui.com/theme-generator/)
  - [Tailwind document](https://tailwindcss.com/docs/installation)
- This project is implemented from scratch by three members of [Group 2](#i-id311-team-project-submission-form).

https://www.bogotobogo.com/cplusplus/sockets_server_client.php