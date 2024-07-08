package main

import (
	"log"
	"math/rand"
	"net/http"
	"time"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
)

type person struct {
	PersonID int    `json:"personID" gorm:"primaryKey;autoIncrement:true"`
	Name     string `json:"personName" gorm:"unique"`
	Status   string `json:"personStatus"`
}

var db *gorm.DB

func initDatabase() {
	var err error
	db, err = gorm.Open(sqlite.Open("people.db"), &gorm.Config{})
	if err != nil {
		log.Fatalf("failed to connect database: %v", err)
	}

	// Run the migration
	if err := db.AutoMigrate(&person{}); err != nil {
		log.Fatalf("failed to migrate schema: %v", err)
	}

	// Optionally, populate the database with initial data if it's empty
	var count int64
	db.Model(&person{}).Count(&count)
	if count == 0 {
		populateDatabase()
	}
}

func populateDatabase() {
	people := []struct {
		Name   string
		Status string
	}{
		{"Elvis Presley", "Dead"},
		{"Michael Jackson", "Dead"},
		{"Freddie Mercury", "Dead"},
		{"Kurt Cobain", "Dead"},
		{"Jim Parsons", "Alive"},
		{"Morgan Freeman", "Alive"},
		{"Tom Hanks", "Alive"},
		{"Kevin Hart", "Alive"},
	}

	for _, p := range people {
		if err := db.Exec("INSERT INTO people (name, status) VALUES (?, ?)", p.Name, p.Status).Error; err != nil {
			log.Printf("Error inserting person %s: %v", p.Name, err)
		}
	}
}

func checkPeople(c *gin.Context) {
	var people []person
	if err := db.Find(&people).Error; err != nil {
		log.Printf("Error fetching people: %v", err)
		c.IndentedJSON(http.StatusInternalServerError, gin.H{"message": "Error fetching people"})
		return
	}
	c.IndentedJSON(http.StatusOK, people)
}

func getRandomPerson(c *gin.Context) {
	var people []person
	if err := db.Find(&people).Error; err != nil {
		log.Printf("Error fetching people: %v", err)
		c.IndentedJSON(http.StatusInternalServerError, gin.H{"message": "Error fetching people"})
		return
	}

	if len(people) == 0 {
		log.Println("No people found")
		c.IndentedJSON(http.StatusNotFound, gin.H{"message": "No people found"})
		return
	}

	random := rand.New(rand.NewSource(time.Now().UnixNano()))
	randomPerson := people[random.Intn(len(people))]
	c.IndentedJSON(http.StatusOK, randomPerson)
}

func submitSelection(c *gin.Context) {
	var selection struct {
		Person1 person `json:"person1"`
		Person2 person `json:"person2"`
		Person3 person `json:"person3"`
	}

	if err := c.BindJSON(&selection); err != nil {
		log.Printf("Invalid JSON: %v", err)
		c.IndentedJSON(http.StatusBadRequest, gin.H{"message": "Invalid JSON"})
		return
	}

	// Debugging: Print received selection
	log.Printf("Received selection: %+v", selection)

	people := []person{selection.Person1, selection.Person2, selection.Person3}
	names := make(map[string]bool)

	for _, p := range people {
		if p.Name == "" || p.Status == "" {
			log.Printf("Missing fields in person: %+v", p)
			c.IndentedJSON(http.StatusBadRequest, gin.H{"message": "Missing fields"})
			return
		}
		if _, exists := names[p.Name]; exists {
			log.Println("Duplicate names")
			c.IndentedJSON(http.StatusBadRequest, gin.H{"message": "Duplicate names"})
			return
		}
		names[p.Name] = true

		var existingPerson person
		if err := db.First(&existingPerson, "name = ?", p.Name).Error; err == nil {
			// Update existing person
			existingPerson.Status = p.Status
			if err := db.Save(&existingPerson).Error; err != nil {
				log.Printf("Error updating person: %v", err)
				c.IndentedJSON(http.StatusInternalServerError, gin.H{"message": "Error updating person"})
				return
			}
		} else {
			// Create new person
			if err := db.Create(&p).Error; err != nil {
				log.Printf("Error creating person: %v", err)
				c.IndentedJSON(http.StatusInternalServerError, gin.H{"message": "Error creating person"})
				return
			}
		}
	}

	c.IndentedJSON(http.StatusCreated, gin.H{"message": "Selection submitted successfully"})
}

func main() {
	initDatabase()

	router := gin.Default()

	router.Use(cors.New(cors.Config{
		AllowOrigins:     []string{"*"},
		AllowMethods:     []string{"GET", "POST", "PUT", "OPTIONS"},
		AllowHeaders:     []string{"Content-Type"},
		AllowCredentials: true,
		MaxAge:           86400,
	}))

	router.GET("/people", checkPeople)
	router.GET("/random", getRandomPerson)
	router.POST("/submit", submitSelection)

	log.Println("Starting server on port 8080...")
	router.Run("0.0.0.0:8080")
}
