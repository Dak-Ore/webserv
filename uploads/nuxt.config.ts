import tailwindcss from "@tailwindcss/vite";
export default defineNuxtConfig({
  compatibilityDate: "2024-11-01",
  devtools: { enabled: true },
  css: ['~/assets/css/main.css'],

  vite: {
    plugins: [
      tailwindcss(),
    ],
  },

  app: {
    pageTransition: { name: 'page', mode: 'out-in' }
  },

  modules: ['@nuxt/image', 'nuxt-time']
});