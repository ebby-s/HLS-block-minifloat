

template <int N, int E, int M>
int bmf_to_bfp_tb(){    // Test BMP to BFP conversion with random stimulus.

    BlockMF<N,E,M>                     bmf_in;    // BMF input.
    BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2> bfp_out;   // BFP output.

    // Log parameters.
    std::cout << "[INFO] BMF to BFP, Parameters: N=" << N << " E=" << E << " M=" << M << '\n';

    for(int l=0; l<(1<<(E+M+1)); l++){    // Test with random inputs.

        // Generate input, corner cases followed by random cases.
        if(l==0){
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    bmf_in.data[j][k].data = 0;
                }
            }
            bmf_in.bias = 0;

        }else{
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    bmf_in.data[j][k].data = rand() % (1<<(E+M+1));
                }
            }
            do{
                bmf_in.bias = rand() % (1<<8);
            }while(((bmf_in.bias+WPRD(E,M))>=128) || ((bmf_in.bias-WPRD(E,M))<-128));
        }

        // Get result from DUT.
        bfp_out = bmf_in;

        // Check if product is normalised.
        bool normalised = false;
        bool zero_data = true;    // Set to false if non-zero output.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                zero_data = (double(bmf_in.data[j][k]) != 0) ? false : zero_data;
                normalised = (bfp_out.data[j][k].acc[WPRD(E,M)/2-1] ^ bfp_out.data[j][k].acc[WPRD(E,M)/2-2]) ? true : normalised;
            }
        }
        if(!zero_data && !normalised){
            printf("[ERROR] FAILED: Product not normalised.\n");
            throw 1;
        }

        // Check if bias was zeroed for cases where all data is zero.
        if(zero_data && (bfp_out.bias != 0)){
            printf("[ERROR] FAILED: Non-zero bias with zero data.\n");
            throw 1;
        }

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((double(bmf_in.data[j][k]) * pow(2,bmf_in.bias)) != (double(bfp_out.data[j][k]) * pow(2,bfp_out.bias))){

                    // std::cout << "Bias (in, out): " << bmf_in.bias << ", " << bfp_out.bias << '\n';
                    // std::cout << "MF (in, out): " << double(bmf_in.data[j][k]) << ", " << double(bfp_out.data[j][k]) << '\n';
                    // std::cout << "Raw (in, out): " << bmf_in.data[j][k].data << ", " << bfp_out.data[j][k].acc << '\n';
                    // std::cout << WPRD(E,M)/2 << '\n';
                    // std::cout << FPRD(E,M)/2 << '\n';

                    printf("[ERROR] FAILED: Reference does not match DUT.\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}


template <int N, int W, int F, int E, int M>
int bfp_to_bmf_tb(){    // Test BFP to BMF conversion with random stimulus.

    BlockFP<N,W,F> bfp_in;    // BFP input.
    BlockMF<N,E,M> bmf_out;   // BMF output.

    // Log parameters.
    std::cout << "[INFO] BFP to BMF, Parameters: N=" << N << " W=" << W << " F=" << F << " E=" << E << " M=" << M << '\n';

    for(int l=0; l<(1<<W); l++){    // Test with random inputs.

        // Generate input randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                bfp_in.data[j][k].acc = rand() % (1<<W);
            }
        }
        do{
            bfp_in.bias = rand() % (1<<8);
        }while(((bfp_in.bias+W-(1<<E)-F)>=128) || ((bfp_in.bias-F)<-128));

        // Get result from DUT.
        bmf_out = bfp_in;

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((rnd_bmf_ninf_u(bfp_in.data[j][k].acc,M,E,W) * pow(2,bfp_in.bias-F)) != (double(bmf_out.data[j][k]) * pow(2,bmf_out.bias))){

                    // std::cout << "Coords: " << j << ", " << k << '\n';
                    // std::cout << "Bias (in, out): " << bfp_in.bias << ", " << bmf_out.bias << '\n';
                    // std::cout << "MF (in, out): " << double(bfp_in.data[j][k]) << ", " << double(bmf_out.data[j][k]) << '\n';
                    // std::cout << "Raw (in, out): " << bfp_in.data[j][k].acc << ", " << bmf_out.data[j][k].data << '\n';
                    // std::cout << "Rounded: " << rnd_bmf_ninf_u(bfp_in.data[j][k].acc,M,E,W) << '\n';
                    // std::cout << WPRD(E,M)/2 << '\n';
                    // std::cout << FPRD(E,M)/2 << '\n';

                    printf("[ERROR] FAILED\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}


template <int N, int W, int F, int Wo, int Fo>
int bfp_to_bfp_tb(){    // Test BFP to BFP conversion with random stimulus.

    BlockFP<N, W, F> bfp_in;    // BFP input.
    BlockFP<N,Wo,Fo> bfp_out;   // BFP output.

    // Log parameters.
    std::cout << "[INFO] BFP to BFP, Parameters: N=" << N << " W=" << W << " F=" << F << " Wo=" << Wo << " Fo=" << Fo << '\n';

    for(int l=0; l<(1<<W); l++){    // Test with random inputs.

        // Generate input randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                bfp_in.data[j][k].acc = rand() % (1<<W);
            }
        }
        do{
            bfp_in.bias = rand() % (1<<8);
        }while(((bfp_in.bias+((W-F)-(Wo-Fo))+1)>=128) || ((bfp_in.bias+((W-F)-(Wo-Fo)))<-128));

        // Get result from DUT.
        bfp_out = bfp_in;

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((rnd_bfp_ninf(bfp_in.data[j][k].acc,W,Wo) * pow(2,bfp_in.bias-F)) != (double(bfp_out.data[j][k]) * pow(2,bfp_out.bias))){

                    std::cout << "Coords: " << j << ", " << k << '\n';
                    std::cout << "Bias (in, out): " << bfp_in.bias << ", " << bfp_out.bias << '\n';
                    std::cout << "MF (in, out): " << double(bfp_in.data[j][k]) << ", " << double(bfp_out.data[j][k]) << '\n';
                    std::cout << "Raw (in, out): " << bfp_in.data[j][k].acc << ", " << bfp_out.data[j][k].acc << '\n';
                    std::cout << "Rounded: " << rnd_bfp_ninf(bfp_in.data[j][k].acc,W,Wo) << '\n';

                    printf("[ERROR] FAILED\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}

