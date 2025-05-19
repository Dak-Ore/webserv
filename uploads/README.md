# Technical Test  
This is my take on the Profil Public technical test.

## Nuxt.js 3 Installation  
I started by installing Node.js v22.15. For the IDE, I chose Visual Studio Code, as it’s the one I use at 42 school and it’s also recommended in the Nuxt “Get Started” guide.

## Job List Creation  

### Using the API  
First, I explored how to use the API to fetch job data. I learned about the `populate` feature in Strapi, which allows including related information such as the job’s location and sector in the response.

### Displaying Details and Filtering  
Next, I created a job details page to display additional information such as `answer1` (possibly the job description) and `answer3` (possibly the requirements).  
I also used REST API `filters` to retrieve specific job data when needed.

### Navigation  
I implemented navigation between the job list and job detail pages using Nuxt's `<NuxtPage>` system. The job ID is passed as a parameter in the route to dynamically load the relevant job information.


## REST API

### Discovering system of pagination
Initially I was thinking that the API had only 25 jobs but when I tried to filter the job by employer I see that there was more than 25 job.
I found how tu use pagination to change page.

### Routing
Remplacing request `GET` by `routes` to have better links.


### WIP

## TODO

- Rework design of details page because it's ugly (text part)
- Finish employer part
- better navbar (true navbar)
- handle case where there is no jobs for a search
- filter system
