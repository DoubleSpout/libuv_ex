{
  "targets":[
    {
      "target_name": "asyn",
      "conditions": [
            ["OS==\"mac\"", {  
                               "sources": ["src/asyn.cc","src/asyn_class.cc","src/job_class.cc"],
                               "libraries": [],
                               "cflags": []
            }],
            ["OS==\"linux\"", {
                               "sources": ["src/asyn.cc","src/asyn_class.cc","src/job_class.cc"],
                               "libraries": [],
                               "cflags": []
            }],
            ["OS==\"win\"", {  
	                       "sources": ["src/asyn.cc","src/asyn_class.cc","src/job_class.cc"],
			                   "libraries": [],
                          "cflags": []
            }]
        ]
    }
  ]
}
