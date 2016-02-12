import os

usage			= "usage : program LearningMethod Feature PositivePath NegativPath SavePath"

base_learner_path 	= "../build-Learner-Desktop_Qt_5_5_0_MSVC2013_64bit-Release/release/Learner.exe"
#base_learner_path	= "\""+base_learner_path+"\"" 

learning_method 	= str(1)
learning_feature	= str(0)

positive_data_path	= "../../BinPicking Images/positive/set3"
positive_data_path	= "\""+positive_data_path+"\"" 

negative_data_path	= "../../BinPicking Images/negative/all"
negative_data_path	= "\""+negative_data_path+"\"" 

save_path		= "./MLP.txt"
save_path		= "\""+save_path+"\"" 

learn_cmd		=  base_learner_path 		+ " "  \
			+ str(learning_method) 	+ " " 	\
			+ str(learning_feature) 		+ " "  	\
			+ positive_data_path 		+ " " 	\
			+ negative_data_path 	+ " " 	\
			+ save_path			
print(learn_cmd)
print(os.execl(base_learner_path, learning_method, learning_feature, positive_data_path, negative_data_path, save_path))