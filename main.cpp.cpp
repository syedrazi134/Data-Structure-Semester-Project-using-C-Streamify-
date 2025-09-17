#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <map>
#include <windows.h>
#include <filesystem>

using namespace std;

void signUp(unordered_map<string, string> &users) {
    string username, password;
    cout << " Enter username: ";
    cin >> username;
    if (users.find(username) != users.end()) {
        cout << " Username already exists. Try again.\n";
        return;
    }
    cout << " Enter password: ";
    cin >> password;
    users[username] = password;
    cout << " Sign-up successful for user: " << username << "\n";
}

bool logIn(unordered_map<string, string> &users, string &currentUser ) {
    string username, password;
    cout << " Enter username: ";
    cin >> username;
    cout << " Enter password: ";
    cin >> password;

    if (users.find(username) != users.end() && users[username] == password) {
        currentUser  = username;
        return true;
    } else {
        cout << " Invalid username or password.\n";
        return false;
    }
}

// Linked List Node for file paths
struct FilePathNode {
    string filePath;
    FilePathNode *next;

    FilePathNode(string path) : filePath(path), next(nullptr) {}
};

// Linked List for managing file paths
class FilePathList {
private:
    FilePathNode *head;
public:
    FilePathList() : head(nullptr) {}

    void addFilePath(const string &path) {
        FilePathNode *newNode = new FilePathNode(path);
        newNode->next = head;
        head = newNode;
    }

    void displayFilePaths() const {
        if (!head) {
            cout << " No file paths available.\n";
            return;
        }
        cout << " File Paths:\n";
        FilePathNode *current = head;
        while (current) {
            cout << "- " << current->filePath << "\n";
            current = current->next;
        }
    }

    string search(int number) {
        if (!head) {
            cout << " No file paths available.\n";
            return " NULL";
        }
        FilePathNode *current = head;
        int i = 1;
        while (current) {
            if (i == number) {
                return current->filePath;
            }
            current = current->next;
            ++i;
        }
        return " NULL";
    }

    ~FilePathList() {
        while (head) {
            FilePathNode *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// AVL Tree Node
struct AVLNode {
    string category;
    FilePathList filePathList;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(const string &cat) : category(cat), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree for managing categories
class AVLTree {
private:
    AVLNode *root;

    int getHeight(AVLNode *node) const {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode *node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    AVLNode *rotateRight(AVLNode *y) {
        AVLNode *x = y->left;
        AVLNode *T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    AVLNode *rotateLeft(AVLNode *x) {
        AVLNode *y = x->right;
        AVLNode *T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    AVLNode *insert(AVLNode *node, const string &category) {
        if (!node)
            return new AVLNode(category);

        if (category < node->category)
            node->left = insert(node->left, category);
        else if (category > node->category)
            node->right = insert(node->right, category);
        else
            return node; // Duplicate categories not allowed

        // Update height
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // Check balance
        int balance = getBalance(node);

        // Balance the tree
        if (balance > 1 && category < node->left->category)
            return rotateRight(node);
        if (balance < -1 && category > node->right->category)
            return rotateLeft(node);
        if (balance > 1 && category > node->left->category) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && category < node->right->category) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void inorder(AVLNode *node) const {
        if (!node)
            return;
        inorder(node->left);
        cout << " Category: " << node->category << "\n";
        node->filePathList.displayFilePaths();
        inorder(node->right);
    }

    AVLNode *find(AVLNode *node, const string &category) {
        if (!node || node->category == category)
            return node;
        if (category < node->category)
            return find(node->left, category);
        return find(node->right, category);
    }

public:
    AVLTree() : root(nullptr) {}

    void addCategory(const string &category) {
        root = insert(root, category);
        cout << " Category added: " << category << "\n";
    }

    void addFilePathToCategory(const string &category, const string &filePath) {
        AVLNode *node = find(root, category);
        if (node) {
            node->filePathList.addFilePath(filePath);
        } else {
            cout << " Category not found: " << category << "\n";
        }
    }

    AVLNode *getCategoryNode(const string &category) {
        return find(root, category);
    }

    void displayCategories() const {
        inorder(root);
    }
};

// Video class definition
class Video {
public:
    string title;
    string filePath;
    int likes;
    vector<string> comments;

    Video() : likes(0) {}

    Video(string title, string filePath) : title(title), filePath(filePath), likes(0) {}

    void addComment(const string &comment) {
        comments.push_back(comment);
    }

    void displayComments() const {
        if (comments.empty()) {
            cout << " No comments yet.\n";
            return;
        }
        cout << " Comments:\n";
        for (const auto &comment : comments) {
            cout << "- " << comment << "\n";
        }
    }
};

// Custom Stack Implementation
template <typename T>
class Stack {
private:
    vector<T> elements;

public:
    void push(T value) {
        elements.push_back(value);
    }

    void pop() {
        if (!elements.empty()) {
            elements.pop_back();
        } else {
            cout << " Stack is empty.\n";
        }
    }

    T top() {
        if (!elements.empty()) {
            return elements.back();
        }
        throw runtime_error(" Stack is empty.");
    }

    bool empty() {
        return elements.empty();
    }

    void display() {
        if (elements.empty()) {
            cout << " No videos watched yet.\n";
            return;
        }
        cout << " Watch History:\n";
        for (int i = elements.size() - 1; i >= 0; --i) {
            cout << "- " << elements[i] << "\n";
        }
    }
};

// Custom Queue Implementation
template <typename T>
class Queue {
private:
    vector<T> elements;

public:
    void enqueue(T value) {
        elements.push_back(value);
 }

    void dequeue() {
        if (!elements.empty()) {
            elements.erase(elements.begin());
        } else {
            cout << " Queue is empty.\n";
        }
    }

    T front() {
        if (!elements.empty()) {
            return elements.front();
        }
        throw runtime_error(" Queue is empty.");
    }

    bool empty() {
        return elements.empty();
    }

    void display() {
        if (elements.empty()) {
            cout << " Playlist is empty.\n";
            return;
        }
        cout << " Playlist:\n";
        for (const auto &video : elements) {
            cout << "- " << video << "\n";
        }
    }
};

// Custom HashMap Implementation
template <typename K, typename V>
class HashMap {
private:
    static const int TABLE_SIZE = 100;
    struct HashNode {
        K key;
        V value;
        HashNode *next;

        HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
    };
    HashNode *table[TABLE_SIZE];

    int hashFunction(const K &key) const {
        return hash<K>{}(key) % TABLE_SIZE;
    }

public:
    HashMap() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode *current = table[i];
            while (current) {
                HashNode *temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const K &key, const V &value) {
        int hashIndex = hashFunction(key);
        HashNode *newNode = new HashNode(key, value);
        if (!table[hashIndex]) {
            table[hashIndex] = newNode;
        } else {
            HashNode *current = table[hashIndex];
            while (current->next) {
                if (current->key == key) {
                    current->value = value; // Update existing key
                    delete newNode;
                    return;
                }
                current = current->next;
            }
            current->next = newNode;
        }
    }

    bool find(const K &key, V &value) const {
        int hashIndex = hashFunction(key);
        HashNode *current = table[hashIndex];
        while (current) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void iterate(function<void(const K &, const V &)> callback) const {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode *current = table[i];
            while (current) {
                callback(current->key, current->value);
                current = current->next;
            }
        }
    }
};

void listVideos(const string &folderPath, vector<string> &videos) {
    // Convert folderPath to wide string
    wstring wideFolderPath(folderPath.begin(), folderPath.end());
    wstring searchPath = wideFolderPath + L"\\*";
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "Failed to open folder: " << folderPath << "\n";
        return;
    }

    do {
        wstring wideFileName = findFileData.cFileName;
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            size_t dotPosition = wideFileName.find_last_of(L".");
            if (dotPosition != wstring::npos) {
                wstring wideExtension = wideFileName.substr(dotPosition + 1);
                if (wideExtension == L"mp4" || wideExtension == L"mkv" || wideExtension == L"avi" || wideExtension == L"wmv") {
                    // Convert wide strings back to narrow strings for the output
                    string fileName(wideFileName.begin(), wideFileName.end());
                    videos.push_back(folderPath + "\\" + fileName);
                }
            }
        }
    } while (FindNextFileW(hFind, &findFileData));

    FindClose(hFind);
}


void playVideo(const string &videoPath) {
    string command = " start wmplayer \"" + videoPath + "\"";
    system(command.c_str());
}

// Updated Creator class
class Creator {
private:
    HashMap<string, Video> &videoLibrary;
    AVLTree &categoryTree;

public:
    Creator(HashMap<string, Video> &vl, AVLTree &ct) : videoLibrary(vl), categoryTree(ct) {}

    void viewVideos() const {
        bool hasVideos = false;
        videoLibrary.iterate([&hasVideos](const string &title, const Video &video) {
            if (!hasVideos) {
                cout << " Uploaded Videos:\n";
                hasVideos = true;
            }
            cout << "- " << title << " (Likes: " << video.likes << ")\n";
            video.displayComments(); // Display comments for each video
        });

        if (!hasVideos) {
            cout << " No videos uploaded yet.\n";
        }
    }

    void uploadVideo(const string &folderPath) {
        vector<string> videos;
        listVideos(folderPath, videos);

        if (videos.empty()) {
            cout << " No videos found in the specified folder.\n";
            return;
        }

        cout << " Videos available for upload:\n";
        for (size_t i = 0; i < videos.size(); ++i) {
            cout << i + 1 << ". " << videos[i].substr(videos[i].find_last_of("\\") + 1) << "\n";
        }

        int choice;
        cout << " Enter the number of the video you want to upload: ";
        cin >> choice;

        if (choice >= 1 && choice <= static_cast<int>(videos.size())) {
            string title = videos[choice - 1].substr(videos[choice - 1].find_last_of("\\") + 1);
            videoLibrary.insert(title, Video(title, videos[choice - 1]));
            cout << " Video uploaded successfully: " << title << "\n";
        } else {
            cout << " Invalid choice.\n";
        }
    }

    void addCategory(const string &category) {
        categoryTree.addCategory(category);
        cout << " Category added: " << category << "\n";
    }

    void uploadVideosToCategory(const string &category, const string &folderPath) {
        AVLNode *node = categoryTree.getCategoryNode(category);
        if (!node) {
            cout << " Category not found: " << category << "\n";
            return;
        }

        vector<string> videos;
        listVideos(folderPath, videos);

        if (videos.empty()) {
            cout << " No videos found in the specified folder.\n";
            return;
        }

        cout << " Videos available for upload:\n";
        for (size_t i = 0; i < videos.size(); ++i) {
            cout << i + 1 << ". " << videos[i].substr(videos[i].find_last_of("\\") + 1) << "\n";
        }

        int choice;
        cout << " Enter the number of the video you want to upload: ";
        cin >> choice;

        if (choice >= 1 && choice <= static_cast<int>(videos.size())) {
            node->filePathList.addFilePath(videos[choice - 1]);
            cout << " Video added to category '" << category << "'.\n";
        } else {
            cout << " Invalid choice.\n";
        }
    }

    void viewCategoryVideos() const {
        cout << " Enter category name to view videos: ";
        string category;
        cin.ignore();
        getline(cin, category);

        AVLNode *node = categoryTree.getCategoryNode(category);
        if (node) {
            cout << " Videos in category '" << category << "':\n";
            node->filePathList.displayFilePaths();
            cout << " Enter the number of video you want to watch: " << endl;
            int choice;
            cin >> choice;
            cin.ignore();
            string file = node->filePathList.search(choice);
            if (file != " NULL") {
                playVideo(file);
            } else {
                cout << " File not found" << endl;
            }
        } else {
            cout << " Category not found.\n";
        }
    }
};

// User class
namespace fs = filesystem;
class User {
private:
    HashMap<string, Video> &videoLibrary;
    Stack<string> watchHistory;
    Queue<string> playlist;

public:
    User(HashMap<string, Video> &vl) : videoLibrary(vl) {}

    void watchVideo(const string &title) {
        Video video;
        if (!videoLibrary.find(title, video)) {
            cout << " Video not found.\n";
            return;
        }

        cout << " Now watching: " << title << "\n";
        playVideo(video.filePath);
        watchHistory.push(title);

        string choice;
        cout << " Would you like to comment on this video? (yes/no): ";
        cin >> choice;
        if (choice == "yes") {
            string comment;
            cout << " Enter your comment: ";
            cin.ignore();
            getline(cin, comment);
            video.addComment(comment);
            cout << " Comment added.\n";
        }

        cout << " Do you want to like this video? (yes/no): ";
        cin >> choice;
        if (choice == "yes") {
            video.likes++;
            cout << " Video liked.\n";
        }

        // Sync changes back to videoLibrary
        videoLibrary.insert(title, video);
    }

    void viewWatchHistory() {
        watchHistory.display();
    }

    void addToPlaylist(const string &title) {
        Video video;
        if (!videoLibrary.find(title, video)) {
            cout << " Video not found.\n";
            return;
        }
        playlist.enqueue(title);
        cout << " Added to playlist: " << title << "\n";
    }

    void watchVideoPlaylist(const string &title) {
        Video video;
        if (!videoLibrary.find(title, video)) {
            cout << " Video not found.\n";
            return;
        }
        playlist.enqueue(title);
    }

    void Queueplaylist() {
        bool hasVideos = false;
        unordered_map<int, string> videoMap; // Map to store video indices and titles
        int index = 1;

        videoLibrary.iterate([&](const string &title, const Video &video) {
            hasVideos = true;
            videoMap[index] = title;
            cout << index << ". " << title << " (Likes: " << video.likes << ")\n";
            index++;
        });

        if (!hasVideos) {
            cout << " No videos available.\n";
            return;
        }

        int choice;
        cout << " Enter the number of the video you want to add: ";
        cin >> choice;

        if (videoMap.find(choice) != videoMap.end()) {
            string selectedVideo = videoMap[choice];
            watchVideoPlaylist(selectedVideo); // Reuse `watchVideo` logic
        } else {
            cout << " Invalid choice. Returning to main menu.\n";
        }
    }

    void playFromPlaylist() {
        if (playlist.empty()) {
            cout << " Playlist is empty.\n";
            return;
        }
        string title = playlist.front();
        playlist.dequeue();

        // Fetch the corresponding video details from the videoLibrary
        Video video;
        if (!videoLibrary.find(title, video)) {
            cout << " Video \"" << title << "\" not found in the library.\n";
            return;
        }

        // Play the video using the full file path
        cout << " Now playing: " << title << "\n";
        playVideo(video.filePath);
    }

    void viewAvailableVideos() {
        bool hasVideos = false;
        unordered_map<int, string> videoMap; // Map to store video indices and titles
        int index = 1;

        videoLibrary.iterate([&](const string &title, const Video &video) {
            hasVideos = true;
            videoMap[index] = title;
            cout << index << ". " << title << " (Likes: " << video.likes << ")\n";
            index++;
        });

        if (!hasVideos) {
            cout << " No videos available.\n";
            return;
        }

        int choice;
        cout << " Enter the number of the video you want to watch: ";
        cin >> choice;

        if (videoMap.find(choice) != videoMap.end()) {
            string selectedVideo = videoMap[choice];
            watchVideo(selectedVideo); // Reuse `watchVideo` logic
        } 
        else {
            cout << " Invalid choice. Returning to main menu.\n";
        }
    }
};

// Main Function
int main() {
    AVLTree categoryTree;
    HashMap<string, Video> videoLibrary;
    Creator creator(videoLibrary, categoryTree);
    User user(videoLibrary);

    unordered_map<string, string> users; // To store usernames and passwords
    bool isAuthenticated = false;       // Authentication flag
    string currentUser ;                 // Stores the username of the logged-in user

    while (!isAuthenticated ) {
        int select;
        cout << " \n=== User Authentication  ===\n";
        cout << " 1. Sign Up\n";
        cout << " 2. Log In\n";
        cout << " 3. Exit\n";
        cout << " Enter your choice: ";
        cin >> select;

        switch (select) {
        case 1:
            signUp(users);
            break;
        case 2:
            if (logIn(users, currentUser )) {
                cout << " Login successful. Welcome, " << currentUser  << "!\n";
                isAuthenticated = true;
            }
            break;
        case 3:
            cout << " Exiting the program. Goodbye!\n";
            return 0;
        default:
            cout << " Invalid choice. Please try again.\n";
        }
    }

    int choice;
    while (true) {
        cout << "\n ====== Home Page ======\n";
        cout << " 1. Creator: Upload Video\n";
        cout << " 2. Creator: View Uploaded Videos\n";
        cout << " 3. User: Watch Video\n";
        cout << " 4. User: View Watch History\n";
        cout << " 5. User: Add to Playlist\n";
        cout << " 6. User: Play from Playlist\n";
        cout << " 7. Category: Add Category\n";
        cout << " 8. Category: Add File Path to Category\n";
        cout << " 9. Category: Display Categories\n";
        cout << " 10. Exit\n";
        cout << " Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            int uploadChoice;
            cout << " 1. Upload video from folder\n";
            cout << " Enter your choice: ";
            cin >> uploadChoice;

            if (uploadChoice == 1) {
                string folderPath;
                cout << "Enter folder path: ";
                cin.ignore();
                getline(cin, folderPath);
                creator.uploadVideo(folderPath);
            } else {
                cout << "Invalid choice.\n";
            }
            break;
        }

        case 2:
            creator.viewVideos();
            break;
        case 3: {
            int viewChoice;
            cout << " 1. View available videos and select one\n";
            cout << " Enter your choice: ";
            cin >> viewChoice;

            if (viewChoice == 1) {
                user.viewAvailableVideos();
            } 
            else {
                cout << " Invalid choice.\n";
            }
            break;
        }

        case 4:
            user.viewWatchHistory();
            break;
        case 5: {
            user.Queueplaylist();
            break;
        }
        case 6:
            user.playFromPlaylist();
            break;
        case 7: {
            string category;
            cout << " Enter the category:\n";
            cin.ignore();
            getline(cin, category);
            creator.addCategory(category);
            break;
        }
        case 8: {
            string category, filePath;
            cout << " Enter category name: ";
            cin.ignore();
            getline(cin, category);
            creator.uploadVideosToCategory(category, "D:\\newFolder");
            break;
        }
        case 9:
            creator.viewCategoryVideos();
            break;
        case 10:
            cout << " Exiting the platform. Goodbye!\n";
            return 0;

        default:
            cout << " Invalid choice. Please try again.\n";
            break;
        }
    }
}